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
{
}

bool SinglePlayerView::Init()
{
	backButton = std::make_shared<Button>(
		"BACK", 1280 / 2 - 150, 500, 300, 100,
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
	backButton->Display(renderEngine);

	return true;
}
