#include "JoinLobbyMenu.h"
#include <game/IGame.h>
#include <game/networking/Lobby.h>
#include <engine/system/Button.h>
#include <engine/core/SDLResourceManager.h>
#include <SDL_opengl.h>

using namespace gogtron;
using namespace gogtron::system;
using namespace gogtron::scene;
using namespace gogtron::networking;

JoinLobbyMenu::JoinLobbyMenu(const IGamePtr& _game)
	: GameState(_game)
	, anyLobbies(true)
{
}

bool JoinLobbyMenu::Init()
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

	backButton = std::make_shared<Button>(
		"button",
		"selectedbutton",
		renderer::Sprite(1280 / 2 - 150, 500, 300, 100),
		[&](){ game->SetGameState(GameState::State::LOBBY_MENU); });

	try
	{
		galaxy::api::Matchmaking()->RequestLobbyList();
	}
	catch (const galaxy::api::IError& error)
	{

	}

	return true;
}

bool JoinLobbyMenu::Release()
{
	return true;
}

void JoinLobbyMenu::OnMouseDown(std::uint32_t x, std::uint32_t y)
{
	backButton->OnMouseDown(x, y);

	for (const auto& element : guiElements)
	{
		element.first->OnMouseDown(x, y);
	}
}

void JoinLobbyMenu::OnMouseMotion(std::uint32_t x, std::uint32_t y)
{
	backButton->OnMouseMotion(x, y);

	for (const auto& element : guiElements)
	{
		element.first->OnMouseMotion(x, y);
	}
}

void JoinLobbyMenu::OnKeyDown(SDL_Keysym key)
{
}

void JoinLobbyMenu::OnLobbyEvent(const LobbyEvent& lobbyEvent)
{
}

bool JoinLobbyMenu::Update()
{
	return true;
}

bool JoinLobbyMenu::Display(const renderer::OGLRendererPtr& renderEngine)
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
	renderEngine->DisplayText("BACK", renderer::Sprite(1280 / 2 - 50, 500, 100, 100), "FreeSans_Back", SDL_Color{ 255, 0, 0, 255 });

	if (!anyLobbies)
	{
		renderEngine->DisplayText("NO LOBBIES AVAILABLE", renderer::Sprite(1280 / 2 - 200, 100, 400, 100), "NoLobbies", SDL_Color{ 255, 0, 0, 255 });
	}
	else
	{
		for (const auto& element : guiElements)
		{
			element.first->Display(renderEngine);
			renderEngine->DisplayText(element.second, renderer::Sprite(1280 / 2 - 150, element.first->GetSprite().GetHeight(), 300, 100), element.second, SDL_Color{ 255, 0, 0, 255 });
		}
	}

	renderEngine->EndScene();
	return true;
}

void JoinLobbyMenu::OnLobbyList(uint32_t lobbyCount, bool ioFailure)
{
	if (!lobbyCount || ioFailure)
	{
		anyLobbies = false;
		return;
	}

	try
	{
		const auto& lobbyID = galaxy::api::Matchmaking()->GetLobbyByIndex(0);

		std::uint32_t posY = 100;

		GUIElementPtr joinButton(std::make_shared<Button>(
			"button",
			"selectedbutton",
			renderer::Sprite(1280 / 2 - 200, posY, 400, 100),
			[&, lobbyID]()
		{
			ILobbyPtr lobby = std::make_shared<Lobby>(game);
			game->SetLobby(lobby);

			try
			{
				galaxy::api::Matchmaking()->JoinLobby(lobbyID);
			}
			catch (const galaxy::api::IError& error)
			{
				return;
			}

			game->SetGameState(GameState::State::IN_LOBBY_MENU);
		}));

		guiElements.push_back(std::make_pair(joinButton, std::to_string(lobbyID.ToUint64())));
	}
	catch (const galaxy::api::IError& error)
	{
		return;
	}
}