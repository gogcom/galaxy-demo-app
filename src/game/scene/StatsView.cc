#include "StatsView.h"
#include <game/IGame.h>
#include <engine/system/Button.h>
#include <engine/core/SDLResourceManager.h>
#include <SDL_opengl.h>

using namespace gogtron;
using namespace gogtron::system;
using namespace gogtron::scene;
using namespace gogtron::networking;

StatsView::StatsView(const IGamePtr& _game)
	: GameState(_game)
{
}

bool StatsView::Init()
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

	if (!core::SDLResourceManager::GetInstance().LoadTexture("res//images//button.png", "button"))
		return false;

	if (!core::SDLResourceManager::GetInstance().LoadTexture("res//images//selectedbutton.png", "selectedbutton"))
		return false;

	if (!core::SDLResourceManager::GetInstance().LoadFont("res//fonts//FreeSans.ttf", "FreeSans"))
		return false;

	GUIElementPtr backButton(std::make_shared<Button>(
		"button",
		"selectedbutton",
		renderer::Sprite(1280 / 2 - 150, 500, 300, 100),
		[&]() { game->SetGameState(GameState::State::START_MENU); }));

	guiElements.push_back(backButton);

	statsRequested = false;

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
	if (!statsRequested)
	{
		try
		{
			galaxy::api::Stats()->RequestUserStatsAndAchievements();
		}
		catch (const galaxy::api::IError&)
		{
			errorMessage = "Failed to get stats";
		}
	}

	statsRequested = true;
	return true;
	}

bool StatsView::Display(const renderer::OGLRendererPtr& renderEngine)
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
	else if (statsRequested)
	{
		renderEngine->DisplayText("User stats:", renderer::Sprite(1280 / 2 - 100, 50, 200, 100), "FreeSans_UserStats", SDL_Color{ 255, 0, 0, 255 });
		const int offsetY = 100;
		int lastY = 50 + offsetY;
		const auto& stats = game->GetGameplayData().GetUserStatistics(galaxy::api::User()->GetGalaxyID());
		for (const auto& stat : stats)
		{
			const auto& statName = stat.second.GetName() + std::string(" ") + std::to_string(galaxy::api::Stats()->GetStatInt(stat.first.c_str()));
			renderEngine->DisplayText(statName, renderer::Sprite(1280 / 2 - 100, lastY, 200, 100), std::string("FreeSans_Stat") + statName, SDL_Color{ 255, 0, 0, 255 });
			lastY += offsetY;
		}
	}
	renderEngine->DisplayText("BACK", renderer::Sprite(1280 / 2 - 50, 500, 100, 100), "FreeSans_Back", SDL_Color{ 255, 0, 0, 255 });

	renderEngine->EndScene();
	return true;
}