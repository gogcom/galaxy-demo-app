#include "StartMenu.h"
#include <game/IGame.h>
#include <engine/system/Button.h>
#include <SDL_opengl.h>

using namespace gogtron;
using namespace gogtron::system;
using namespace gogtron::scene;
using namespace gogtron::networking;

StartMenu::StartMenu(const IGamePtr& _game)
	: GameState(_game)
{
}

bool StartMenu::Init()
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
		"PLAY", 1280 / 2 - 150, 25, 300, 100,
		[&]() { game->SetGameState(GameState::State::SINGLE_PLAYER_VIEW); }));

	guiElements.emplace_back(std::make_shared<Button>(
		"LOBBY", 1280 / 2 - 150, 125, 300, 100,
		/*Sprite(400, 100, 400, 224),*/
		[&]() { game->SetGameState(GameState::State::LOBBY_MENU); }));

	guiElements.emplace_back(std::make_shared<Button>(
		"STATS", 1280 / 2 - 150, 225, 300, 100,
		[&]() { game->SetGameState(GameState::State::STATS_VIEW); }));

	guiElements.emplace_back(std::make_shared<Button>(
		"LEADERBOARDS", 1280 / 2 - 150, 325, 300, 100,
		[&]() { game->SetGameState(GameState::State::LEADERBOARDS_VIEW); }));

	guiElements.emplace_back(std::make_shared<Button>(
		"CLOUD STORAGE", 1280 / 2 - 150, 425, 300, 100,
		[&]() { game->SetGameState(GameState::State::CLOUD_STORAGE_VIEW); }));

	guiElements.emplace_back(std::make_shared<Button>(
		"QUIT", 1280 / 2 - 150, 550, 300, 100,
		/*Sprite(400, 400, 400, 224),*/
		[&]() { game->Close(); }));

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
		element->OnMouseDown(x, y);
	}
}

void StartMenu::OnMouseMotion(std::uint32_t x, std::uint32_t y)
{
	for (const auto& element : guiElements)
	{
		element->OnMouseMotion(x, y);
	}
}

void StartMenu::OnKeyDown(SDL_Keysym key)
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

void StartMenu::OnLobbyEvent(const LobbyEvent& lobbyEvent)
{
}

bool StartMenu::Update()
{
	return true;
}

bool StartMenu::Display(const renderer::OGLRendererPtr& renderEngine)
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

	renderEngine->EndScene();
	return true;
}