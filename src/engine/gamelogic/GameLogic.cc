#include "GameLogic.h"

#include <algorithm>

namespace galaxy::demo::gamelogic
{

	namespace
	{
		constexpr uint8_t MIN_PLAYERS_TO_PLAY = 2;
		constexpr uint8_t MAX_WINNERS = 1;
	}

	GameLogic::GameLogic(PlayerPtrList _players)
		: players{std::move(_players)}
	{
		assert(players.size() >= MIN_PLAYERS_TO_PLAY);
	}

	void GameLogic::Tick()
	{
		UpdatePlayersPositions();
		CheckPlayersCollisions();
	}

	bool GameLogic::UpdatePlayersPositions()
	{
		for (const auto& player : players)
		{
			if (!player->IsAlive())
				continue;

			const auto& lastPlayerPosition = player->GetPosition();
			const auto& playerDirection = player->GetDirection();
			const auto& lastPlayerDirection = player->GetLastDirection();
			const auto& lastTailPartPosition = player->GetLastTailPosition();

			const auto newPlayerPosition = glm::vec3(
				lastPlayerPosition.x + lastPlayerDirection.x * 0.125f,
				lastPlayerPosition.y,
				lastPlayerPosition.z + lastPlayerDirection.y * 0.125f);

			if (lastPlayerDirection.x)
			{
				if (std::fabsf(newPlayerPosition.x - lastTailPartPosition.x) == 1.0f)
				{
					player->AddTailPart(glm::vec3(lastTailPartPosition.x, lastTailPartPosition.y, lastTailPartPosition.z));
					player->SetLastTailPosition(glm::vec3(lastTailPartPosition.x + lastPlayerDirection.x, lastTailPartPosition.y, lastTailPartPosition.z));
					player->SetLastDirection(playerDirection);
				}
			}
			else if (lastPlayerDirection.y)
			{
				if (std::fabsf(newPlayerPosition.z - lastTailPartPosition.z) == 1.0f)
				{
					player->AddTailPart(glm::vec3(lastTailPartPosition.x, lastTailPartPosition.y, lastTailPartPosition.z));
					player->SetLastTailPosition(glm::vec3(lastTailPartPosition.x, lastTailPartPosition.y, lastTailPartPosition.z + lastPlayerDirection.y));
					player->SetLastDirection(playerDirection);
				}
			}

			player->SetPosition(newPlayerPosition);
		}

		return true;
	}

	bool GameLogic::CheckPlayersCollisions()
	{
		for (const auto& player : players)
		{
			if (!player->IsAlive())
				continue;

			const auto& playerPosition = player->GetPosition();

			if (playerPosition.x >= 50 || playerPosition.x <= -50
				|| playerPosition.z >= 50 || playerPosition.z <= -50)
			{
				player->SetIsAlive(false);
				continue;
			}

			for (auto& otherPlayer : players)
			{
				if (!otherPlayer->IsAlive())
					continue;

				const auto& otherPlayerPosition = otherPlayer->GetPosition();

				if (player->GetGalaxyID() != otherPlayer->GetGalaxyID()
					&& std::fabsf(playerPosition.x - otherPlayerPosition.x) < 0.2f
					&& std::fabsf(playerPosition.z - otherPlayerPosition.z) <= 0.2f)
				{
					player->SetIsAlive(false);
					otherPlayer->SetIsAlive(false);
					continue;
				}

				const auto& playerTail = otherPlayer->GetTail();
				for (const auto& tailPart : playerTail)
				{
					if (std::fabsf(playerPosition.x - tailPart->GetPosition().x) < 0.2f
						&& std::fabsf(playerPosition.z - tailPart->GetPosition().z) <= 0.2f)
					{
						player->SetIsAlive(false);
						if (player->GetGalaxyID() != otherPlayer->GetGalaxyID())
							otherPlayer->SetPoints(otherPlayer->GetPoints() + 10);
						break;
					}
				}
			}
		}

		return true;
	}

	bool GameLogic::IsWinnerDefined() const
	{
		assert(players.size() >= 0);

		return std::count_if(std::cbegin(players), std::cend(players), [](const auto& player) {
			return player->IsAlive();
		}) <= MAX_WINNERS;
	}

}