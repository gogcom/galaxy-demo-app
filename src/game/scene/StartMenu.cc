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

using namespace galaxy::demo;
using namespace galaxy::demo::system;
using namespace galaxy::demo::scene;
using namespace galaxy::demo::networking;

namespace
{
	constexpr auto BUTTON_HEIGHT = 80;
	constexpr galaxy::api::ProductID DLCProductID = 1109610972U;

	inline GalaxyStatus GetCurrentStatus()
	{
		if (!galaxy::api::IsFullyInitialized)
			return GalaxyStatus::NotInitialized;

		if (galaxy::api::User()->IsLoggedOn())
			return GalaxyStatus::SignedInOnline;

		if (galaxy::api::User()->SignedIn())
			return GalaxyStatus::SignedInOffline;

		return GalaxyStatus::SignedOff;
	}

	inline std::shared_ptr<Button> MakeButton(std::string label, int offsetY, system::GUINotification action)
	{
		return std::make_shared<Button>(std::move(label), 1280 / 2 - 150, offsetY, 300, BUTTON_HEIGHT, std::move(action));
	}
}

StartMenu::StartMenu(const IGamePtr& _game)
	: GameState(_game)
{
}

bool StartMenu::Init()
{
	int offsetY = 25;
	guiElements.emplace_back(MakeButton("PLAY", offsetY, [&]() { game->SetGameState(GameState::State::SINGLE_PLAYER_VIEW); }), GalaxyStatus::Any);

	offsetY += BUTTON_HEIGHT;
	guiElements.emplace_back(MakeButton("Logging in...", offsetY, [&]() {}), GalaxyStatus::NotInitialized | GalaxyStatus::SignedOff);

	guiElements.emplace_back(MakeButton("LOBBY", offsetY, [&]() { game->SetGameState(GameState::State::LOBBY_MENU); }), GalaxyStatus::SignedInOnline);

	// TODO: add feature from IApps
	// TODO: add filesystem features

	offsetY += BUTTON_HEIGHT;
	guiElements.emplace_back(MakeButton("STATS", offsetY, [&]() { game->SetGameState(GameState::State::STATS_VIEW); }), GalaxyStatus::SignedInOffline | GalaxyStatus::SignedInOnline);

	offsetY += BUTTON_HEIGHT;
	guiElements.emplace_back(MakeButton("LEADERBOARDS", offsetY, [&]() { game->SetGameState(GameState::State::LEADERBOARDS_VIEW); }), GalaxyStatus::SignedInOffline | GalaxyStatus::SignedInOnline);

	offsetY += BUTTON_HEIGHT;
	guiElements.emplace_back(MakeButton("STORAGE", offsetY, [&]() { game->SetGameState(GameState::State::CLOUD_STORAGE_VIEW); }), GalaxyStatus::SignedInOffline | GalaxyStatus::SignedInOnline);

	offsetY += BUTTON_HEIGHT;
	guiElements.emplace_back(MakeButton("CHAT", offsetY, [&]() { game->SetGameState(GameState::State::CHAT_VIEW); }), GalaxyStatus::SignedInOffline | GalaxyStatus::SignedInOnline);

	offsetY += BUTTON_HEIGHT * 2;
	guiElements.emplace_back(MakeButton("QUIT", offsetY, [&]() { game->Close(); }), GalaxyStatus::Any);

	if(galaxy::api::Apps()->IsDlcInstalled(DLCProductID))
		dlcStatus = "DLC 1 installed.";

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

	renderEngine->DisplayText(dlcStatus, renderer::Sprite(100, 600, dlcStatus.length() * 10, 50), "FreeSans_" + dlcStatus, SDL_Color{ 255, 0, 0, 255 });

	return true;
}
