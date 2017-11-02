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
	static inline GalaxyStatus GetCurrentStatus()
	{
		if (!galaxy::api::IsFullyInitialized)
			return GalaxyStatus::NotInitialized;

		if (galaxy::api::User()->IsLoggedOn())
			return GalaxyStatus::SignedInOnline;

		if (galaxy::api::User()->SignedIn())
			return GalaxyStatus::SignedInOffline;

		return GalaxyStatus::SignedOff;
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
		GalaxyStatus::Any
	});

	guiElements.push_back({std::make_shared<Button>("Logging in...", 1280 / 2 - 150, 125, 300, 100, [&]() {}),
		GalaxyStatus::NotInitialized | GalaxyStatus::SignedOff
	});

	guiElements.push_back({std::make_shared<Button>("LOBBY", 1280 / 2 - 150, 125, 300, 100, [&]() { game->SetGameState(GameState::State::LOBBY_MENU); }),
		GalaxyStatus::SignedInOnline
	});

	// TODO: add feature from IApps
	// TODO: add filesystem features

	guiElements.push_back({std::make_shared<Button>("STATS", 1280 / 2 - 150, 225, 300, 100, [&]() { game->SetGameState(GameState::State::STATS_VIEW); }),
		GalaxyStatus::SignedInOffline | GalaxyStatus::SignedInOnline
	});

	guiElements.push_back({std::make_shared<Button>("LEADERBOARDS", 1280 / 2 - 150, 325, 300, 100, [&]() { game->SetGameState(GameState::State::LEADERBOARDS_VIEW); }),
		GalaxyStatus::SignedInOffline | GalaxyStatus::SignedInOnline
	});

	guiElements.push_back({std::make_shared<Button>("CLOUD STORAGE", 1280 / 2 - 150, 425, 300, 100, [&]() { game->SetGameState(GameState::State::CLOUD_STORAGE_VIEW); }),
		GalaxyStatus::SignedInOffline | GalaxyStatus::SignedInOnline
	});

	guiElements.push_back({std::make_shared<Button>("QUIT", 1280 / 2 - 150, 550, 300, 100, [&]() { game->Close(); }),
		GalaxyStatus::Any
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
		if (element.second & GetCurrentStatus())
			element.first->OnMouseDown(x, y);
	}
}

void StartMenu::OnMouseMotion(std::uint32_t x, std::uint32_t y)
{
	for (const auto& element : guiElements)
	{
		if (element.second & GetCurrentStatus())
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
		if (element.second & GetCurrentStatus())
			element.first->Display(renderEngine);
	}

	return true;
}
