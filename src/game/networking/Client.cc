#include "Client.h"
#include <game/IGame.h>
#include <game/messages/Ready.h>
#include <game/messages/InitGame.h>
#include <game/messages/GameTick.h>
#include <game/messages/UpdateDirection.h>
#include <game/messages/GameResults.h>
#include <engine/serialize/JsonSerializer.h>
#include <algorithm>

using namespace gogtron;
using namespace gogtron::networking;
using namespace gogtron::scene;

Client::Client(const IGamePtr& _game)
	: IClient(_game)
{
}

bool Client::SendReadyToServer()
{
	message::Ready ready;
	std::string readyMessage;
	if (!serialize::JsonSerializer::Serialize(&ready, readyMessage))
		return false;

	return SendMessageToServer(readyMessage);
}

bool Client::SendUpdateDirectionToServer(const glm::vec2& direction)
{
	message::UpdateDirection updateDirection(direction);
	std::string message;
	if (!serialize::JsonSerializer::Serialize(&updateDirection, message))
		return false;

	return SendMessageToServer(message);
}

void Client::OnP2PPacketAvailable(uint32_t msgSize, uint8_t channel)
{
	char message[4096] = {'\0'};
	std::uint32_t messageSize;
	galaxy::api::GalaxyID userID;
	if (!galaxy::api::Networking()->PeekP2PPacket(message, sizeof(message), &messageSize, userID, channel))
		return;

	switch (game->GetGameManager().GetClientState())
	{
		case GameManager::ClientState::INIT:
		{
			message::Ready ready;
			message::InitGame initGame;
			if (serialize::JsonSerializer::Deserialize(&ready, message))
			{
				auto& gameManager = game->GetGameManager();
				auto& players = gameManager.GetPlayers();
				const auto& playerFound = std::find_if(std::begin(players), std::end(players), [&](const PlayerPtr& p) {return p->GetGalaxyID() == ready.GetPlayerID(); });
				if (playerFound == std::end(players))
					return;

				(*playerFound)->SetIsReady(true);
			}
			else if (serialize::JsonSerializer::Deserialize(&initGame, message))
			{
				auto& gameManager = game->GetGameManager();
				for (const auto& player : initGame.GetPlayers())
				{
					gameManager.AddPlayer(player);
				}

				game->SetGameState(GameState::State::IN_GAME);
				game->GetGameManager().SetClientState(GameManager::ClientState::GAME);
			}
		}
		break;

		case GameManager::ClientState::START_GAME:
		{
			/*
			message::InitGame initGame;
			if (message::JsonSerializer::Deserialize(&initGame, message))
			{
				auto& gameManager = game->GetGameManager();
				for (const auto& player : initGame.GetPlayers())
				{
					gameManager.AddPlayer(player);
				}

				game->GetGameManager().SetClientState(GameManager::ClientState::GAME);
			}
			*/
		}
		break;

		case GameManager::ClientState::GAME:
		{
			message::GameTick gameTick;
			message::GameResults gameResults;
			if (serialize::JsonSerializer::Deserialize(&gameTick, message))
			{
				auto& gameManager = game->GetGameManager();
				auto& players = gameManager.GetPlayers();
				for (const auto& player : gameTick.GetPlayers())
				{
					const auto& playerFound = std::find_if(std::begin(players), std::end(players), [&](const PlayerPtr& p) {return p->GetGalaxyID() == player->GetGalaxyID(); });
					if (playerFound == std::end(players))
						continue;

					(*playerFound)->SetIsAlive(player->IsAlive());

					(*playerFound)->SetPosition(player->GetPosition());
					(*playerFound)->SetDirection(player->GetDirection());
					(*playerFound)->SetPoints(player->GetPoints());

					const auto& lastTailPosition = player->GetLastTailPosition();
					const auto& tailPosition = (*playerFound)->GetLastTailPosition();

					if (lastTailPosition != tailPosition)
						(*playerFound)->AddTailPart(lastTailPosition);
				}
			}
			else if (serialize::JsonSerializer::Deserialize(&gameResults, message))
			{
				auto& gameManager = game->GetGameManager();
				gameManager.SetGameTime(gameResults.GetGameTime());
				auto& players = gameManager.GetPlayers();
				for (const auto& player : gameResults.GetPlayers())
				{
					const auto& playerFound = std::find_if(std::begin(players), std::end(players), [&](const PlayerPtr& p) {return p->GetGalaxyID() == player->GetGalaxyID(); });
					if (playerFound == std::end(players))
						continue;

					(*playerFound)->SetPoints(player->GetPoints());
				}

				gameManager.SetClientState(GameManager::ClientState::RESULT);
				game->SetGameState(GameState::State::GAME_RESULT);
			}
		}
		break;

		case GameManager::ClientState::RESULT:
			break;

		default:
			break;
	}
}
