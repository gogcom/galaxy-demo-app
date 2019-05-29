#include "JoinLobbyMenu.h"
#include <game/IGame.h>
#include <game/networking/Lobby.h>
#include <engine/system/Button.h>
#include <SDL_opengl.h>

using namespace galaxy::demo;
using namespace galaxy::demo::system;
using namespace galaxy::demo::scene;
using namespace galaxy::demo::networking;

JoinLobbyMenu::JoinLobbyMenu(const IGamePtr& _game)
	: GameState(_game)
	, anyLobbies(true)
{
}

bool JoinLobbyMenu::Init()
{
	backButton = std::make_shared<Button>(
		"BACK", 1280 / 2 - 150, 500, 300, 100,
		[&]() { game->SetGameState(GameState::State::LOBBY_MENU); });

	try
	{
		galaxy::api::Matchmaking()->RequestLobbyList();
	}
	catch (const galaxy::api::IError& /*error*/)
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
		element->OnMouseDown(x, y);
	}
}

void JoinLobbyMenu::OnMouseMotion(std::uint32_t x, std::uint32_t y)
{
	backButton->OnMouseMotion(x, y);

	for (const auto& element : guiElements)
	{
		element->OnMouseMotion(x, y);
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
	// Refresh lobby list once in a while
	static int refreshCount = 0;
	if (refreshCount++ > 50)
	{
		galaxy::api::Matchmaking()->RequestLobbyList();
		refreshCount = 0;
	}

	if (!anyLobbies)
	{
		renderEngine->DisplayText("NO LOBBIES AVAILABLE", renderer::Sprite(1280 / 2 - 200, 100, 400, 100), "NoLobbies", SDL_Color{255, 0, 0, 255});
	}
	else
	{
		for (const auto& element : guiElements)
		{
			element->Display(renderEngine);
		}
	}

	backButton->Display(renderEngine);
	return true;

}

void JoinLobbyMenu::OnLobbyList(uint32_t lobbyCount, galaxy::api::LobbyListResult result)
{
	if (!lobbyCount || result != galaxy::api::LOBBY_LIST_RESULT_SUCCESS)
	{
		anyLobbies = false;
		return;
	}

	guiElements.clear();

	for (uint32_t idx{0}; idx < lobbyCount; ++idx)
	{
		try
		{
			const auto& lobbyID = galaxy::api::Matchmaking()->GetLobbyByIndex(idx);

			std::uint32_t posY = 100 * idx;

			guiElements.emplace_back(std::make_shared<Button>(
				std::to_string(lobbyID.ToUint64()), 1280 / 2 - 200, posY, 400, 100,
				[&, lobbyID]()
			{
				ILobbyPtr lobby = std::make_shared<Lobby>(game);
				game->SetLobby(lobby);

				try
				{
					galaxy::api::Matchmaking()->JoinLobby(lobbyID);
				}
				catch (const galaxy::api::IError& /*error*/)
				{
					return;
				}

				game->SetGameState(GameState::State::IN_LOBBY_MENU);
			}));
		}
		catch (const galaxy::api::IError& /*error*/)
		{
			return;
		}

		anyLobbies = true;
	}
}