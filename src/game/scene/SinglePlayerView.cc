#include "SinglePlayerView.h"
#include <game/IGame.h>
#include <engine/system/Button.h>
#include <SDL_opengl.h>

using namespace gogtron;
using namespace gogtron::system;
using namespace gogtron::scene;
using namespace gogtron::networking;

SinglePlayerView::SinglePlayerView(const IGamePtr& _game)
	: GameState(_game)
	, anyLobbies(true)
{
}

bool SinglePlayerView::Init()
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

	backButton = std::make_shared<Button>(
		"button",
		"selectedbutton",
		renderer::Sprite(1280 / 2 - 150, 500, 300, 100),
		[&]() { game->SetGameState(GameState::State::START_MENU); });

	// TODO: add single player logic here

	return true;
}

bool SinglePlayerView::Release()
{
	return true;
}

void SinglePlayerView::OnMouseDown(std::uint32_t x, std::uint32_t y)
{
	backButton->OnMouseDown(x, y);

	for (const auto& element : guiElements)
	{
		element.first->OnMouseDown(x, y);
	}
}

void SinglePlayerView::OnMouseMotion(std::uint32_t x, std::uint32_t y)
{
	backButton->OnMouseMotion(x, y);

	for (const auto& element : guiElements)
	{
		element.first->OnMouseMotion(x, y);
	}
}

void SinglePlayerView::OnKeyDown(SDL_Keysym key)
{
}

void SinglePlayerView::OnLobbyEvent(const LobbyEvent& lobbyEvent)
{
}

bool SinglePlayerView::Update()
{
	return true;
}

bool SinglePlayerView::Display(const renderer::OGLRendererPtr& renderEngine)
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

	backButton->Display(renderEngine);
	renderEngine->DisplayText("BACK", renderer::Sprite(1280 / 2 - 50, 500, 100, 100), "FreeSans_Back", SDL_Color{255, 0, 0, 255});

	renderEngine->EndScene();
	return true;
}
