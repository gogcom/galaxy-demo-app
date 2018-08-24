#include "LeaderboardsView.h"
#include <game/IGame.h>
#include <engine/system/Button.h>
#include <SDL_opengl.h>

using namespace galaxy::demo;
using namespace galaxy::demo::system;
using namespace galaxy::demo::scene;
using namespace galaxy::demo::networking;

LeaderboardsView::LeaderboardsView(const IGamePtr& _game)
	: GameState(_game)
{
}

bool LeaderboardsView::Init()
{
	guiElements.emplace_back(std::make_shared<Button>(
		"BACK", 1280 / 2 - 150, 500, 300, 100,
		[&]() { game->SetGameState(GameState::State::START_MENU); }));

	game->GetGameplayData().RefreshLeaderboardsEntries();

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
	return true;
}

bool LeaderboardsView::Display(const renderer::OGLRendererPtr& renderEngine)
{
	for (const auto& element : guiElements)
	{
		element->Display(renderEngine);
	}

	if (!errorMessage.empty())
	{
		renderEngine->DisplayText(errorMessage.c_str(), renderer::Sprite(50, 100, 300, 100), "LobbyMenuErrorMessage", SDL_Color{255, 0, 0, 255});
	}

	renderEngine->DisplayText("Leaderboards:", renderer::Sprite(1280 / 2 - 100, 50, 200, 100), "FreeSans_BestLeaderboards", SDL_Color{255, 0, 0, 255});
	const int offsetY = 100;
	const int offsetX = 250;
	int lastY = 50 + offsetY;
	const auto& leaderboards = game->GetGameplayData().GetLeaderboards();
	for (const auto& leaderboard : leaderboards)
	{
		int lastX = 50;
		const auto& leaderboardName = leaderboard.second.displayName;
		renderEngine->DisplayText(leaderboardName + ":", renderer::Sprite(lastX, lastY, 200, 100), std::string("FreeSans_Leaderboard") + leaderboardName, SDL_Color{255, 0, 0, 255});

		const auto& entries = leaderboard.second.entries;
		for (const auto& entry : entries)
		{
			lastX += offsetX;
			const auto displayText = std::string(galaxy::api::Friends()->GetFriendPersonaName(entry.userID)) + " - " + std::to_string(entry.score);
			renderEngine->DisplayText(displayText, renderer::Sprite(lastX, lastY, 200, 100), std::string("FreeSans_LeaderboardEntry") + displayText, SDL_Color{255, 0, 0, 255});
		}

		lastY += offsetY;
	}

	return true;
}