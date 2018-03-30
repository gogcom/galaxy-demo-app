#include "LobbyHost.h"

#include <engine/serialize/JsonSerializer.h>
#include <game/messages/GameTick.h>
#include <game/messages/GameResults.h>
#include <game/messages/InitGame.h>
#include <game/messages/Ready.h>
#include <game/messages/UpdateDirection.h>

#include <glm/mat3x4.hpp>
#include <glm/detail/type_vec.hpp>
#include <game/Player.h>

#include <galaxy/GalaxyExceptionHelper.h>

#include <algorithm>
#include <chrono>

#include <iterator>
#include <utility>

namespace
{

	constexpr uint32_t MIN_PLAYERS_TO_PLAY = 2;
	constexpr uint32_t MAX_PLAYERS = 4;

}

namespace gogtron::server
{

	LobbyHost::LobbyHost(const galaxy::api::GalaxyID& _lobbyID)
		: lobbyID{_lobbyID}
		, state{State::WAITING_FOR_PLAYERS}
		, startGameTime{0}
	{
		assert(lobbyID.IsValid());
	}

	LobbyHost::State LobbyHost::GetHostState() const
	{
		return state;
	}

	void LobbyHost::SetHostState(State _state)
	{
		state = _state;
	}

	void LobbyHost::OnLobbyMemberStateChanged(const galaxy::api::GalaxyID& _lobbyID, const galaxy::api::GalaxyID& memberID, galaxy::api::LobbyMemberStateChange memberStateChange)
	{
		if (lobbyID != _lobbyID)
			// Callback for another lobby
			return;

		switch (memberStateChange)
		{
			case galaxy::api::LOBBY_MEMBER_STATE_CHANGED_LEFT:
			case galaxy::api::LOBBY_MEMBER_STATE_CHANGED_DISCONNECTED:
			case galaxy::api::LOBBY_MEMBER_STATE_CHANGED_KICKED:
			case galaxy::api::LOBBY_MEMBER_STATE_CHANGED_BANNED:
			{
				auto removedMemberCount = membersMap.erase(memberID);
				assert(removedMemberCount == 1);
				(void)removedMemberCount;

				galaxy::api::GameServerLogger()->Info("Member left: lobbyID=%llu, memberID=%llu, memberCount=%u", lobbyID.ToUint64(), memberID.ToUint64(), membersMap.size());

				switch (GetHostState())
				{
					case State::IN_GAME:
					{
						if (gameLogic->IsWinnerDefined())
							FinishGame();

						return;
					}
					case State::FINISHED:
					{
						if (membersMap.size() < MIN_PLAYERS_TO_PLAY)
							LeaveLobby();

						return;
					}
					default:
						return;
				}
			}

			case galaxy::api::LOBBY_MEMBER_STATE_CHANGED_ENTERED:
			{
				auto insertResult = membersMap.emplace(std::make_pair(memberID, std::make_shared<Player>(memberID)));
				assert(insertResult.second);
				(void)insertResult;

				galaxy::api::GameServerLogger()->Info("Member joined: lobbyID=%llu, memberID=%llu, memberCount=%u", lobbyID.ToUint64(), memberID.ToUint64(), membersMap.size());
				return;
			}
		}
	}

	void LobbyHost::OnServerP2PPacketAvailable(uint32_t msgSize, uint8_t channel)
	{
		assert(msgSize);

		static char message[4096] = {'\0'};

		std::uint32_t messageSize;
		galaxy::api::GalaxyID userID;
		if (!galaxy::api::GameServerNetworking()->PeekP2PPacket(message, sizeof(message), &messageSize, userID, channel))
			return;

		if (membersMap.find(userID) == std::end(membersMap))
		{
			galaxy::api::GameServerLogger()->Warning("Received a packet from an user who is not a lobby member: userID=%llu, lobbyID=%llu", userID.ToUint64(), lobbyID.ToUint64());
			assert(false);
			return;
		}

		switch (GetHostState())
		{
			case State::WAITING_FOR_PLAYERS:
			{
				message::Ready ready;
				if (!serialize::JsonSerializer::Deserialize(&ready, message))
				{
					galaxy::api::GameServerLogger()->Warning("Received unknown message while waiting for 'Ready' from players");
					assert(false);
					return;
				}

				OnMemberReady(userID);
			}
			break;

			case State::IN_GAME:
			{
				message::UpdateDirection updateDirection;
				if (!serialize::JsonSerializer::Deserialize(&updateDirection, message))
					return;

				auto memberIt = membersMap.find(userID);
				if (memberIt == std::end(membersMap))
					return;

				memberIt->second->SetDirection(updateDirection.GetDirection());
			}
			break;

			case State::FINISHED:
				break;

			default:
				break;
		}
	}

	void LobbyHost::OnMemberReady(const galaxy::api::GalaxyID& memberID)
	{
		assert(memberID.IsValid());

		galaxy::api::GameServerLogger()->Info("Member is ready: memberID=%llu, memberCount=%u", memberID.ToUint64(), membersMap.size());

		membersMap.at(memberID)->SetIsReady(true);

		if (ReadyToStart() && StartGame())
			SetHostState(State::IN_GAME);
	}

	bool LobbyHost::ReadyToStart() const
	{
		if (membersMap.size() < MIN_PLAYERS_TO_PLAY)
			return false;

		for (const auto& member : membersMap)
		{
			if (member.second->IsReady() == false)
				return false;
		}

		return true;
	}

	bool LobbyHost::StartGame()
	{
		assert(gameLogic.get() == nullptr);
		gameLogic = std::make_unique<gamelogic::GameLogic>(GetPlayers());

		std::string initGameMessage{CreateInitGameMessage()};
		if (initGameMessage.empty())
		{
			galaxy::api::GameServerLogger()->Error("Failed to create 'InitGame' message: lobbyID=%llu, playerCount=%u", lobbyID.ToUint64(), membersMap.size());
			return false;
		}

		BroadcastMessage(initGameMessage);

		startGameTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		galaxy::api::GameServerLogger()->Info("Starting a game: lobbyID=%llu, playerCount=%u", lobbyID.ToUint64(), membersMap.size());

		return true;
	}

	std::string LobbyHost::CreateInitGameMessage() const
	{
		const glm::vec4 playerColors[4] = {
			glm::vec4(1, 0, 0, 1),
			glm::vec4(0, 1, 0, 1),
			glm::vec4(0, 0, 1, 1),
			glm::vec4(1, 1, 0, 1)
		};

		const glm::vec3 playerPositions[4] = {
			glm::vec3(0.5, 0, -24.5),
			glm::vec3(0.5, 0, 24.5),
			glm::vec3(-24.5, 0, 0.5),
			glm::vec3(24.5, 0, -0.5)
		};

		const glm::vec2 playerDirections[4] = {
			glm::vec2(0, 1),
			glm::vec2(0, -1),
			glm::vec2(1, 0),
			glm::vec2(-1, 0)
		};


		int index = 0;
		for (const auto& memberIt : membersMap)
		{
			auto& player = memberIt.second;

			player->SetPosition(playerPositions[index]);
			player->SetDirection(playerDirections[index]);
			player->SetColor(playerColors[index]);
			player->SetLastDirection(playerDirections[index]);
			player->SetLastTailPosition(playerPositions[index]);
			player->SetIsAlive(true);
			player->SetPoints(0);

			if (index == 3)
				break;
			++index;
		}

		std::string initGameMessage;
		message::InitGame initGame{GetPlayers()};
		serialize::JsonSerializer::Serialize(&initGame, initGameMessage);

		return initGameMessage;
	}

	void LobbyHost::Tick()
	{
		if (GetHostState() == State::IN_GAME)
		{
			assert(membersMap.size() >= MIN_PLAYERS_TO_PLAY);

			gameLogic->Tick();

			if (gameLogic->IsWinnerDefined())
				FinishGame();

			SendGameTick();
		}
	}


	void LobbyHost::SendGameTick()
	{
		std::string message;
		message::GameTick gameTick{GetPlayers()};
		if (!serialize::JsonSerializer::Serialize(&gameTick, message))
		{
			galaxy::api::GameServerLogger()->Error("Failed to serialize 'GameTick' message: lobbyID=%llu, playerCount=%u", lobbyID.ToUint64(), membersMap.size());
			return;
		}

		BroadcastMessage(message);
	}

	void LobbyHost::SendGameResults()
	{
		uint64_t gameTime{std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() - startGameTime};

		assert(gameTime > 0);

		std::string message;
		message::GameResults gameResults{GetPlayers(), gameTime};
		if (!serialize::JsonSerializer::Serialize(&gameResults, message))
		{
			galaxy::api::GameServerLogger()->Error("Failed to serialize 'GameResults' message: lobbyID=%llu, playerCount=%u, gameTime=%llu", lobbyID.ToUint64(), membersMap.size(), gameTime);
			return;
		}

		galaxy::api::GameServerLogger()->Info("Broadcasting game results: lobbyID=%llu, playerCount=%u, gameTime=%llu", lobbyID.ToUint64(), membersMap.size(), gameTime);
		BroadcastMessage(message);
	}

	void LobbyHost::FinishGame()
	{
		SetHostState(State::FINISHED);
		SendGameResults();
	}

	void LobbyHost::BroadcastMessage(const std::string& message) const
	{
		assert(membersMap.size() >= MIN_PLAYERS_TO_PLAY);
		assert(!message.empty());

		for (const auto& member : membersMap)
		{
			assert(member.first.IsValid());
			galaxy::api::GameServerNetworking()->SendP2PPacket(member.first, message.c_str(), message.size(), galaxy::api::P2P_SEND_RELIABLE_IMMEDIATE);
		}
	}

	std::vector<PlayerPtr> LobbyHost::GetPlayers() const
	{
		assert(!membersMap.empty());

		std::vector<PlayerPtr> players;
		for (auto& memberIt : membersMap)
			players.push_back(memberIt.second);

		return players;
	}

	void LobbyHost::LeaveLobby() const
	{
		galaxy::api::GameServerMatchmaking()->LeaveLobby(lobbyID);
	}

}