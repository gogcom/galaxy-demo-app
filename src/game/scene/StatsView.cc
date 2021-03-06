#include "StatsView.h"
#include <game/IGame.h>
#include <engine/system/Button.h>
#include <SDL_opengl.h>

using namespace galaxy::demo;
using namespace galaxy::demo::system;
using namespace galaxy::demo::scene;
using namespace galaxy::demo::networking;

StatsView::StatsView(const IGamePtr& _game)
	: GameState(_game)
{
}

bool StatsView::Init()
{
	guiElements.emplace_back(std::make_shared<Button>(
		"BACK", 1280 / 2 - 150, 500, 300, 100,
		[&]() { game->SetGameState(GameState::State::START_MENU); }));

	try
	{
		galaxy::api::Stats()->RequestUserStatsAndAchievements();
	}
	catch (const galaxy::api::IError&)
	{
		errorMessage = "Failed to get stats";
	}

	return true;
}

bool StatsView::Release()
{
	return true;
}

void StatsView::OnMouseDown(std::uint32_t x, std::uint32_t y)
{
	for (const auto& element : guiElements)
	{
		element->OnMouseDown(x, y);
	}
}

void StatsView::OnMouseMotion(std::uint32_t x, std::uint32_t y)
{
	for (const auto& element : guiElements)
	{
		element->OnMouseMotion(x, y);
	}
}

void StatsView::OnKeyDown(SDL_Keysym key)
{
}

void StatsView::OnLobbyEvent(const LobbyEvent& lobbyEvent)
{
}

bool StatsView::Update()
{
	return true;
}

bool StatsView::Display(const renderer::OGLRendererPtr& renderEngine)
{
	for (const auto& element : guiElements)
	{
		element->Display(renderEngine);
	}

	if (!errorMessage.empty())
	{
		renderEngine->DisplayText(errorMessage.c_str(), renderer::Sprite(50, 100, 300, 100), "LobbyMenuErrorMessage", SDL_Color{255, 0, 0, 255});
	}
	else
	{
		renderEngine->DisplayText("User stats:", renderer::Sprite(1280 / 2 - 100, 50, 200, 100), "FreeSans_UserStats", SDL_Color{255, 0, 0, 255});
		const int offsetY = 100;
		int lastY = 50 + offsetY;
		const auto& stats = game->GetGameplayData().GetUserStatistics(galaxy::api::User()->GetGalaxyID());
		for (const auto& stat : stats)
		{
			const auto& statName = stat.second.GetName() + std::string(" ") + std::to_string(stat.second.GetInt());
			renderEngine->DisplayText(statName, renderer::Sprite(1280 / 2 - 100, lastY, 200, 100), std::string("FreeSans_Stat") + statName, SDL_Color{255, 0, 0, 255});
			lastY += offsetY;
		}
	}

	return true;
}