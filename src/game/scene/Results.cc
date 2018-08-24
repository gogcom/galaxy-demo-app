#include "Results.h"
#include <game/IGame.h>
#include <game/networking/Server.h>
#include <game/networking/Client.h>
#include <engine/system/Button.h>
#include <SDL_opengl.h>
#include <algorithm>

using namespace galaxy::demo;
using namespace galaxy::demo::system;
using namespace galaxy::demo::scene;
using namespace galaxy::demo::networking;

Results::Results(const IGamePtr& _game)
	: GameState(_game)
{
}

bool Results::Init()
{
	guiElements.push_back(std::make_shared<Button>(
		"BACK", 1280 / 2 - 150, 500, 300, 100,
		[&]() { game->SetGameState(GameState::State::JOIN_LOBBY_MENU); }));

	resultsStored = false;
	return true;
}

bool Results::Release()
{
	return true;
}

void Results::OnMouseDown(std::uint32_t x, std::uint32_t y)
{
	for (const auto& element : guiElements)
	{
		element->OnMouseDown(x, y);
	}
}

void Results::OnMouseMotion(std::uint32_t x, std::uint32_t y)
{
	for (const auto& element : guiElements)
	{
		element->OnMouseMotion(x, y);
	}
}

void Results::OnKeyDown(SDL_Keysym key)
{
	switch (key.sym)
	{
		case SDLK_UP:
			guiElements[0]->OnMouseMotion(450, 150);
			break;

		case SDLK_DOWN:
			break;

		case SDLK_KP_ENTER:
			guiElements[0]->OnMouseDown(450, 150);
			break;

		default:
			break;
	}
}

void Results::OnLobbyEvent(const LobbyEvent& lobbyEvent)
{
}

bool Results::Update()
{
	if (resultsStored)
		return true;

	const auto& gameManager = game->GetGameManager();
	auto players = gameManager.GetPlayers();
	std::sort(std::begin(players), std::end(players), [](const PlayerPtr& first, const PlayerPtr& second) { return first->GetPoints() > second->GetPoints(); });
	if (players.empty())
		return true;

	const auto& winner = *(players.begin());

	{
		try
		{
			const auto& stats = game->GetGameplayData().GetUserStatistics(galaxy::api::User()->GetGalaxyID());
			const auto& statistic = stats.find("ride_count");
			if (statistic != std::end(stats))
				galaxy::api::Stats()->SetStatInt("ride_count", statistic->second.GetInt() + 1);
		}
		catch (const galaxy::api::IError& er)
		{
			galaxy::api::Logger()->Error("Failed to get user stats: ", er.GetMsg());
		}
	}

	if (winner->GetGalaxyID() == galaxy::api::User()->GetGalaxyID())
	{
		auto& achievements = game->GetGameplayData().GetUserAchievements(galaxy::api::User()->GetGalaxyID());
		{
			try
			{
				const auto& achievement = achievements.find("first_win");
				if (achievement != std::end(achievements) && !achievement->second.unlocked)
					galaxy::api::Stats()->SetAchievement("first_win");
			}
			catch (const galaxy::api::IError& er)
			{
				galaxy::api::Logger()->Error("Failed to set 'first_win' achievement: ", er.GetMsg());
			}
		}

		{
			try
			{
				const auto& achievement = achievements.find("quick_win");
				const int quickWinTimeout = 15;
				if (achievement != std::end(achievements) && !achievement->second.unlocked && game->GetGameManager().GetGameTime() < quickWinTimeout)
					galaxy::api::Stats()->SetAchievement("quick_win");
			}
			catch (const galaxy::api::IError& er)
			{
				galaxy::api::Logger()->Error("Failed to set 'quick_win' achievement: ", er.GetMsg());
			}
		}

		auto& stats = game->GetGameplayData().GetUserStatistics(galaxy::api::User()->GetGalaxyID());
		{
			try
			{
				const auto& statistic = stats.find("win_count");
				if (statistic != std::end(stats))
					galaxy::api::Stats()->SetStatInt("win_count", statistic->second.GetInt() + 1);
			}
			catch (const galaxy::api::IError& er)
			{
				galaxy::api::Logger()->Error("Failed to set stat 'win_count': ", er.GetMsg());
			}
		}

		auto& leaderboards = game->GetGameplayData().GetLeaderboards();
		{
			const auto& leaderboard = leaderboards.find("best_winners");
			if (leaderboard != std::end(leaderboards))
			{
				try
				{
					const auto& entries = leaderboard->second.entries;
					const auto entry = std::find_if(std::begin(entries), std::end(entries), [](const Leaderboard::Entry& entry) { return entry.userID == galaxy::api::User()->GetGalaxyID(); });
					int score = entry != std::end(entries) ? entry->score + 1 : 1;
					galaxy::api::Stats()->SetLeaderboardScore("best_winners", score);
				}
				catch (const galaxy::api::IError& er)
				{
					galaxy::api::Logger()->Error("Failed to set leaderboard score 'best_winners': ", er.GetMsg());
				}
			}
		}

		{
			const auto& leaderboard = leaderboards.find("quickest_winners");
			if (leaderboard != std::end(leaderboards))
			{
				try
				{
					const auto& entries = leaderboard->second.entries;
					const auto entry = std::find_if(std::begin(entries), std::end(entries), [](const Leaderboard::Entry& entry) { return entry.userID == galaxy::api::User()->GetGalaxyID(); });
					uint64_t gameTime = game->GetGameManager().GetGameTime();
					if (entry == std::end(entries))
						galaxy::api::Stats()->SetLeaderboardScore("quickest_winners", static_cast<uint32_t>(gameTime), true);
					else if (gameTime < entry->score || entry->score == 0)
						galaxy::api::Stats()->SetLeaderboardScore("quickest_winners", static_cast<uint32_t>(gameTime), true);
				}
				catch (const galaxy::api::IError& er)
				{
					galaxy::api::Logger()->Error("Failed to set leaderboard score 'quickest_winners': ", er.GetMsg());
				}
			}
		}
	}

	try
	{
		galaxy::api::Stats()->StoreStatsAndAchievements();
	}
	catch (const galaxy::api::IError& er)
	{
		galaxy::api::Logger()->Error("Failed to store stats and achievements: ", er.GetMsg());
	}

	resultsStored = true;
	return true;
}

bool Results::Display(const renderer::OGLRendererPtr& renderEngine)
{
	for (const auto& element : guiElements)
	{
		element->Display(renderEngine);
	}

	renderEngine->DisplayText("Results:", renderer::Sprite(1280 / 2 - 50, 50, 100, 100), "FreeSans_Results", SDL_Color{255, 0, 0, 255});
	const int offsetY = 100;
	int lastY = 50 + offsetY;
	const auto& gameManager = game->GetGameManager();
	auto players = gameManager.GetPlayers();
	std::sort(std::begin(players), std::end(players), [](const PlayerPtr& first, const PlayerPtr& second) { return first->GetPoints() > second->GetPoints(); });
	for (const auto& player : players)
	{
		const auto& playerResult = galaxy::api::Friends()->GetFriendPersonaName(player->GetGalaxyID()) + std::string(" : ") + std::to_string(player->GetPoints());
		renderEngine->DisplayText(playerResult, renderer::Sprite(50, lastY, 300, 100), std::string("FreeSans_result") + playerResult, SDL_Color{255, 0, 0, 255});
		lastY += offsetY;
	}

	return true;
}
