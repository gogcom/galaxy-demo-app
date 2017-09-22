#include "LeaderboardsView.h"
#include <game/IGame.h>
#include <engine/system/Button.h>
#include <SDL_opengl.h>

using namespace gogtron;
using namespace gogtron::system;
using namespace gogtron::scene;
using namespace gogtron::networking;

LeaderboardsView::LeaderboardsView(const IGamePtr& _game)
	: GameState(_game)
{
}

bool LeaderboardsView::Init()
{
	glViewport(0, 0, 1280, 720);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1280, 720, 1.0, -1.0, 1.0);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1280, 720, 1.0, -1.0, 1.0);

	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	guiElements.emplace_back(std::make_shared<Button>(
		"BACK", 1280 / 2 - 150, 500, 300, 100,
		[&]() { game->SetGameState(GameState::State::START_MENU); }));

	leaderboardsRequested = false;
	leaderboardEntriesRequested = false;

	return true;
}

bool LeaderboardsView::Release()
{
	return true;
}

void LeaderboardsView::OnMouseDown(std::uint32_t x, std::uint32_t y)
{
	for (const auto& element : guiElements)
	{
		element->OnMouseDown(x, y);
	}
}

void LeaderboardsView::OnMouseMotion(std::uint32_t x, std::uint32_t y)
{
	for (const auto& element : guiElements)
	{
		element->OnMouseMotion(x, y);
	}
}

void LeaderboardsView::OnKeyDown(SDL_Keysym key)
{
}

void LeaderboardsView::OnLobbyEvent(const LobbyEvent& lobbyEvent)
{
}

bool LeaderboardsView::Update()
{
	if (!leaderboardsRequested)
	{
		try
		{
			galaxy::api::Stats()->RequestLeaderboards();
		}
		catch (const galaxy::api::IError&)
		{
			errorMessage = "Failed to get leaderboards";
		}

		leaderboardsRequested = true;
	}
	else if (leaderboardsRequested && !leaderboardEntriesRequested)
	{
		const auto& leaderboards = game->GetGameplayData().GetLeaderboards();
		for (const auto& leaderboard : leaderboards)
		{
			try
			{
				galaxy::api::Stats()->RequestLeaderboardEntriesGlobal(leaderboard.first.c_str(), 0, 3);
			}
			catch (const galaxy::api::IError&)
			{
				errorMessage = "Failed to get leaderboards";
			}
		}
		leaderboardEntriesRequested = true;
	}

	return true;
}

bool LeaderboardsView::Display(const renderer::OGLRendererPtr& renderEngine)
{
	glViewport(0, 0, 1280, 720);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1280, 720, 1.0, -1.0, 1.0);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1280, 720, 1.0, -1.0, 1.0);

	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	renderEngine->StartScene();

	for (const auto& element : guiElements)
	{
		element->Display(renderEngine);
	}


	if (!errorMessage.empty())
	{
		renderEngine->DisplayText(errorMessage.c_str(), renderer::Sprite(50, 100, 300, 100), "LobbyMenuErrorMessage", SDL_Color{ 255, 0, 0, 255 });
	}
	else if (leaderboardEntriesRequested)
	{
		renderEngine->DisplayText("Leaderboards:", renderer::Sprite(1280 / 2 - 100, 50, 200, 100), "FreeSans_BestLeaderboards", SDL_Color{ 255, 0, 0, 255 });
		const int offsetY = 100;
		const int offsetX = 250;
		int lastY = 50 + offsetY;
		const auto& leaderboards = game->GetGameplayData().GetLeaderboards();
		for (const auto& leaderboard : leaderboards)
		{
			int lastX = 50;
			const auto& leaderboardName = leaderboard.second.displayName;
			renderEngine->DisplayText(leaderboardName + ":", renderer::Sprite(lastX, lastY, 200, 100), std::string("FreeSans_Leaderboard") + leaderboardName, SDL_Color{ 255, 0, 0, 255 });

			const auto& entries = leaderboard.second.entries;
			for (const auto& entry : entries)
			{
				lastX += offsetX;
				const auto displayText = std::string(galaxy::api::Friends()->GetFriendPersonaName(entry.userID)) + " - " + std::to_string(entry.score);
				renderEngine->DisplayText(displayText, renderer::Sprite(lastX, lastY, 200, 100), std::string("FreeSans_LeaderboardEntry") + displayText, SDL_Color{ 255, 0, 0, 255 });
			}

			lastY += offsetY;
		}
	}

	renderEngine->EndScene();
	return true;
}