#include "InitFailedView.h"
#include <game/IGame.h>
#include <engine/system/Button.h>
#include <SDL_opengl.h>
#include <algorithm>

using namespace galaxy::demo;
using namespace galaxy::demo::system;
using namespace galaxy::demo::scene;

InitFailedView::InitFailedView(const IGamePtr& _game)
	: GameState(_game)
{
}

bool InitFailedView::Init()
{
	guiElements.emplace_back(std::make_shared<Button>(
		"EXIT", 1280 / 2 - 150, 500, 300, 100,
		[&]() { game->Close(); }));
	return true;
}

bool InitFailedView::Release()
{
	return true;
}

void InitFailedView::OnMouseDown(std::uint32_t x, std::uint32_t y)
{
	for (const auto& element : guiElements)
	{
		element->OnMouseDown(x, y);
	}
}

void InitFailedView::OnMouseMotion(std::uint32_t x, std::uint32_t y)
{
	for (const auto& element : guiElements)
	{
		element->OnMouseMotion(x, y);
	}
}

void InitFailedView::OnKeyDown(SDL_Keysym key)
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

void InitFailedView::OnLobbyEvent(const networking::LobbyEvent& lobbyEvent)
{
}

bool InitFailedView::Update()
{
	return true;
}

bool InitFailedView::Display(const renderer::OGLRendererPtr& renderEngine)
{
	for (const auto& element : guiElements)
	{
		element->Display(renderEngine);
	}

	renderEngine->DisplayText("FAILED TO INITIALIZE", renderer::Sprite(1280 / 2 - 200, 100, 400, 100), "FailedToInit", SDL_Color{255, 0, 0, 255});

	return true;
}
