#include "StartMenu.h"
#include <game/IGame.h>
#include <engine/system/Button.h>
#include <galaxy/GalaxyApi.h>
#include <SDL_opengl.h>

namespace galaxy
{
	namespace api
	{
		extern bool IsFullyInitialized;
	}
}

using namespace gogtron;
using namespace gogtron::system;
using namespace gogtron::scene;
using namespace gogtron::networking;

namespace
{
	static inline GalaxyStatus GetCombinedStatus()
	{
		if (!galaxy::api::IsFullyInitialized)
			return GalaxyStatus::NotInitialized;

		if (!galaxy::api::User()->SignedIn())
			return GalaxyStatus::Offline;

		return GalaxyStatus::SignedIn;
	}
}

StartMenu::StartMenu(const IGamePtr& _game)
	: GameState(_game)
{
}

bool StartMenu::Init()
{
	guiElements.push_back({
		std::make_shared<Button>("PLAY", 1280 / 2 - 150, 25, 300, 100, [&]() { game->SetGameState(GameState::State::SINGLE_PLAYER_VIEW); }),
		GalaxyStatus::NotInitialized | GalaxyStatus::Offline | GalaxyStatus::SignedIn
	});

	guiElements.push_back({std::make_shared<Button>("LOBBY", 1280 / 2 - 150, 125, 300, 100, [&]() { game->SetGameState(GameState::State::LOBBY_MENU); }),
		GalaxyStatus::SignedIn
	});

	// TODO: add feature from IApps
	// TODO: add filesystem features

	guiElements.push_back({std::make_shared<Button>("STATS", 1280 / 2 - 150, 225, 300, 100, [&]() { game->SetGameState(GameState::State::STATS_VIEW); }),
		GalaxyStatus::Offline | GalaxyStatus::SignedIn
	});

	guiElements.push_back({std::make_shared<Button>("LEADERBOARDS", 1280 / 2 - 150, 325, 300, 100, [&]() { game->SetGameState(GameState::State::LEADERBOARDS_VIEW); }),
		GalaxyStatus::Offline | GalaxyStatus::SignedIn
	});

	guiElements.push_back({std::make_shared<Button>("CLOUD STORAGE", 1280 / 2 - 150, 425, 300, 100, [&]() { game->SetGameState(GameState::State::CLOUD_STORAGE_VIEW); }),
		GalaxyStatus::Offline | GalaxyStatus::SignedIn
	});

	guiElements.push_back({std::make_shared<Button>("QUIT", 1280 / 2 - 150, 550, 300, 100, [&]() { game->Close(); }),
		GalaxyStatus::NotInitialized | GalaxyStatus::Offline | GalaxyStatus::SignedIn
	});

	return true;
}

bool StartMenu::Release()
{
	return true;
}

void StartMenu::OnMouseDown(std::uint32_t x, std::uint32_t y)
{
	for (const auto& element : guiElements)
	{
		if (element.second & GetCombinedStatus())
			element.first->OnMouseDown(x, y);
	}
}

void StartMenu::OnMouseMotion(std::uint32_t x, std::uint32_t y)
{
	for (const auto& element : guiElements)
	{
		if (element.second & GetCombinedStatus())
			element.first->OnMouseMotion(x, y);
	}
}

void StartMenu::OnKeyDown(SDL_Keysym key)
{
	switch (key.sym)
	{
		case SDLK_UP:
			guiElements[0].first->OnMouseMotion(450, 150);
			break;

		case SDLK_DOWN:
			break;

		case SDLK_KP_ENTER:
			guiElements[0].first->OnMouseDown(450, 150);
			break;

		default:
			break;
	}
}

void StartMenu::OnLobbyEvent(const LobbyEvent& lobbyEvent)
{
}

bool StartMenu::Update()
{
	return true;
}

bool StartMenu::Display(const renderer::OGLRendererPtr& renderEngine)
{
	for (const auto& element : guiElements)
	{
		if (element.second & GetCombinedStatus())
			element.first->Display(renderEngine);
	}

	return true;
}
