#include "InsideLobbyMenu.h"
#include <game/IGame.h>
#include <game/networking/Lobby.h>
#include <game/networking/Server.h>
#include <game/networking/Client.h>
#include <engine/system/Button.h>
#include <SDL_opengl.h>

using namespace galaxy::demo;
using namespace galaxy::demo::system;
using namespace galaxy::demo::scene;
using namespace galaxy::demo::networking;

namespace
{

	constexpr uint32_t MIN_PLAYERS_TO_START_GAME = 2;

}

InsideLobbyMenu::InsideLobbyMenu(const IGamePtr& _game)
	: GameState(_game)
{
}

bool InsideLobbyMenu::Init()
{
	guiElements.emplace_back(std::make_shared<Button>(
		"PLAY", 1280 / 2 - 150, 100, 300, 100,
		[&]()
	{
		const auto& client = game->GetClient();
		if (!client)
			return;

		if (!client->SendReadyToServer())
			return;
	}));

	guiElements.emplace_back(std::make_shared<Button>(
		"INVITE FRIEND", 1280 / 2 - 150, 300, 300, 100,
		[&]()
	{
		try
		{
			const auto& lobby = game->GetLobby();
			const auto connectionString = lobby->GetConnectionString();
			galaxy::api::Friends()->ShowOverlayInviteDialog(connectionString.c_str());
		}
		catch (const galaxy::api::IError&)
		{

		}
	}));

	guiElements.emplace_back(std::make_shared<Button>(
		"BACK", 1280 / 2 - 150, 500, 300, 100,
		[&]()
	{
		game->SetClient(nullptr);
		game->SetServer(nullptr);
		game->SetLobby(nullptr);
		game->SetGameState(GameState::State::LOBBY_MENU);
	}));

	auto& gameManager = game->GetGameManager();
	gameManager.SetClientState(GameManager::ClientState::INIT);
	gameManager.SetServerState(GameManager::ServerState::INIT);

	const auto& server = game->GetServer();
	if (server)
	{
		server->SetPlayers({});
	}

	gameManager.GetPlayers().clear();

	return true;
}

bool InsideLobbyMenu::Release()
{
	return true;
}

void InsideLobbyMenu::OnMouseDown(std::uint32_t x, std::uint32_t y)
{
	for (const auto& element : guiElements)
	{
		element->OnMouseDown(x, y);
	}
}

void InsideLobbyMenu::OnMouseMotion(std::uint32_t x, std::uint32_t y)
{
	for (const auto& element : guiElements)
	{
		element->OnMouseMotion(x, y);
	}
}

void InsideLobbyMenu::OnKeyDown(SDL_Keysym key)
{
}

void InsideLobbyMenu::OnLobbyEvent(const LobbyEvent& lobbyEvent)
{
	switch (lobbyEvent)
	{
		case LobbyEvent::LOBBY_CREATE_FAILURE:
			break;

		case LobbyEvent::LOBBY_CREATE_SUCCESS:
		{
			auto server = std::make_shared<networking::Server>(game);
			game->SetServer(server);
			game->GetGameManager().SetServerState(GameManager::ServerState::INIT);
		}
		break;

		case LobbyEvent::LOBBY_JOIN_FAILURE:
			break;

		case LobbyEvent::LOBBY_JOIN_SUCCESS:
		{
			auto client = std::make_shared<networking::Client>(game);
			client->SetServerID(game->GetLobby()->GetLobbyID());
			game->SetClient(client);
			game->GetGameManager().SetClientState(GameManager::ClientState::INIT);
		}
		break;

		default:
			break;
	}
}

bool InsideLobbyMenu::Update()
{
	const auto& server = game->GetServer();
	if (server)
	{
		switch (game->GetGameManager().GetServerState())
		{
			case GameManager::ServerState::INIT:
			{
				if (!server->RetrievedReadyFromEachClient())
					break;

				const auto& lobbyMembers = game->GetLobby()->GetLobbyMembers();
				if (lobbyMembers.size() < MIN_PLAYERS_TO_START_GAME)
					break;

				if (!server->SendInitGame())
					break;

				server->InitGameLogic();
				game->GetGameManager().SetServerState(GameManager::ServerState::GAME);
			}
			break;

			default:
				break;
		}
	}

	return true;
}

bool InsideLobbyMenu::Display(const renderer::OGLRendererPtr& renderEngine)
{
	for (const auto& element : guiElements)
	{
		element->Display(renderEngine);
	}

	renderEngine->DisplayText("Lobby members:", renderer::Sprite(50, 50, 200, 100), "FreeSans_Nicknames", SDL_Color{ 255, 0, 0, 255 });
	const int offsetY = 100;
	int lastY = 50 + offsetY;
	const auto& lobbyMembers = game->GetLobby()->GetLobbyMembers();
	for (const auto& lobbyMember : lobbyMembers)
	{
		const char* lobbyMemberNickname = NULL;
		try
		{
			lobbyMemberNickname = galaxy::api::Friends()->GetFriendPersonaName(lobbyMember);
		}
		catch (const galaxy::api::IError& /*error*/)
		{
			lobbyMemberNickname = "unknown";
		}

		renderEngine->DisplayText(lobbyMemberNickname, renderer::Sprite(50, lastY, 100, 100), std::string("FreeSans_Nickname") + lobbyMemberNickname, SDL_Color{ 255, 0, 0, 255 });
		lastY += offsetY;
	}

	return true;
}