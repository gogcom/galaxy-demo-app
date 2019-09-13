#include "Server.h"
#include <game/IGame.h>
#include <game/messages/Ready.h>
#include <game/messages/InitGame.h>
#include <game/messages/GameTick.h>
#include <game/messages/UpdateDirection.h>
#include <game/messages/GameResults.h>
#include <engine/serialize/JsonSerializer.h>
#include <cstdint>
#include <algorithm>

using namespace galaxy::demo;
using namespace galaxy::demo::networking;

Server::Server(const IGamePtr& game)
	: IServer(game)
{
}

bool Server::SendMessageToAll(const std::string& message) const
{
	const auto& players = game->GetLobby()->GetLobbyMembers();
	for (const auto& player : players)
		if (!galaxy::api::Networking()->SendP2PPacket(player, message.c_str(), message.size(), galaxy::api::P2P_SEND_RELIABLE))
			return false;
	return true;
}

bool Server::RetrievedReadyFromEachClient() const
{
	const auto& playerCount = game->GetLobby()->GetLobbyMembers().size();
	/*
	if (playerCount == 1)
	{
		//game->SetGameState(GameState::State::IN_LOBBY_MENU);
		return false;
	}
	*/
	return playerCount == messages.size();
}

bool Server::SendInitGame()
{
	std::string initGame;
	if (!CreateInitGame(initGame))
		return false;

	if (!SendMessageToAll(initGame))
		return false;

	messages.clear();
	return true;
}

bool Server::SendGameTick(const std::vector<PlayerPtr>& _players)
{
	std::string message;
	message::GameTick gameTick(_players);
	if (!serialize::JsonSerializer::Serialize(&gameTick, message))
		return false;

	if (!SendMessageToAll(message))
		return false;

	players = _players;

	return true;
}

bool Server::SendGameResults(const std::vector<PlayerPtr>& _players, uint64_t gameTime)
{
	std::string message;
	message::GameResults gameResults(_players, gameTime);
	if (!serialize::JsonSerializer::Serialize(&gameResults, message))
		return false;

	if (!SendMessageToAll(message))
		return false;

	players.clear();
	game->GetGameManager().SetServerState(GameManager::ServerState::RESULTS);
	return true;
}

void Server::OnP2PPacketAvailable(uint32_t msgSize, uint8_t channel)
{
	char message[4096] = {'\0'};
	std::uint32_t messageSize;
	galaxy::api::GalaxyID userID;
	if (!galaxy::api::Networking()->PeekP2PPacket(message, sizeof(message), &messageSize, userID, channel))
		return;

	switch (game->GetGameManager().GetServerState())
	{
		case GameManager::ServerState::INIT:
		{
			if (messages.find(userID) != std::end(messages))
				return;

			message::Ready ready;
			if (!serialize::JsonSerializer::Deserialize(&ready, message))
				return;

			messages[userID].push_back(message);
		}
		break;

		case GameManager::ServerState::GAME:
		{
			message::UpdateDirection updateDirection;
			if (!serialize::JsonSerializer::Deserialize(&updateDirection, message))
				return;

			const auto& player = std::find_if(std::begin(players), std::end(players), [&userID](const PlayerPtr& player) { return player->GetGalaxyID() == userID; });
			if (player == std::end(players))
				return;

			(*player)->SetDirection(updateDirection.GetDirection());
		}
		break;

		case GameManager::ServerState::RESULTS:
			break;

		default:
			break;
	}
}

bool Server::CreateInitGame(std::string& message)
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
	const auto& members = game->GetLobby()->GetLobbyMembers();
	for (const auto& member : members)
	{
		auto player = std::make_shared<Player>(
			member,
			playerPositions[index],
			playerDirections[index],
			playerColors[index],
			playerDirections[index],
			playerPositions[index],
			true,
			0);

		AddPlayer(player);

		if (index == 3)
			break;
		++index;
	}

	message::InitGame initGame(players);
	return serialize::JsonSerializer::Serialize(&initGame, message);
}
