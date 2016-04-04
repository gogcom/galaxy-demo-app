#include "InsideLobbyMenu.h"
#include <game/IGame.h>
#include <game/networking/Lobby.h>
#include <game/networking/Server.h>
#include <game/networking/Client.h>
#include <engine/system/Button.h>
#include <engine/core/SDLResourceManager.h>
#include <SDL_opengl.h>

using namespace gogtron;
using namespace gogtron::system;
using namespace gogtron::scene;
using namespace gogtron::networking;

InsideLobbyMenu::InsideLobbyMenu(const IGamePtr& _game)
	: GameState(_game)
{
}

bool InsideLobbyMenu::Init()
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

	GUIElementPtr playButton(std::make_shared<Button>(
		"button",
		"selectedbutton",
		renderer::Sprite(1280 / 2 - 150, 100, 300, 100),
		[&]()
	{
		const auto& client = game->GetClient();
		if (!client)
			return;

		if (!client->SendReadyToServer())
			return;
	}));

	guiElements.push_back(playButton);

	GUIElementPtr inviteFriend(std::make_shared<Button>(
		"button",
		"selectedbutton",
		renderer::Sprite(1280 / 2 - 150, 300, 300, 100),
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

	guiElements.push_back(inviteFriend);

	GUIElementPtr backButton(std::make_shared<Button>(
		"button",
		"selectedbutton",
		renderer::Sprite(1280 / 2 - 150, 500, 300, 100),
		[&]()
	{
		game->SetClient(nullptr);
		game->SetServer(nullptr);
		game->SetLobby(nullptr);
		game->SetGameState(GameState::State::LOBBY_MENU);
	}));

	guiElements.push_back(backButton);

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
			if (lobbyMembers.size() <= 1)
				break;

			if (!server->SendInitGame())
				break;

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
	renderEngine->StartScene();

	for (const auto& element : guiElements)
	{
		element->Display(renderEngine);
	}

	renderEngine->DisplayText("PLAY", renderer::Sprite(1280 / 2 - 50, 100, 100, 100), "FreeSans_PlayGame", SDL_Color{ 255, 0, 0, 255 });
	renderEngine->DisplayText("INVITE FRIEND", renderer::Sprite(1280 / 2 - 100, 300, 200, 100), "FreeSans_InviteFriend", SDL_Color{ 255, 0, 0, 255 });
	renderEngine->DisplayText("BACK", renderer::Sprite(1280 / 2 - 50, 500, 100, 100), "FreeSans_Back", SDL_Color{ 255, 0, 0, 255 });

	renderEngine->DisplayText("Lobby members:", renderer::Sprite(50, 50, 200, 100), "FreeSans_Nicknames", SDL_Color{ 255, 0, 0, 255 });
	const int offsetY = 100;
	int lastY = 50 + offsetY;
	const auto& lobbyMembers = game->GetLobby()->GetLobbyMembers();
	for (const auto& lobbyMember : lobbyMembers)
	{
		const auto& lobbyMemberNickname = galaxy::api::Friends()->GetFriendPersonaName(lobbyMember);
		renderEngine->DisplayText(lobbyMemberNickname, renderer::Sprite(50, lastY, 100, 100), std::string("FreeSans_Nickname") + lobbyMemberNickname, SDL_Color{ 255, 0, 0, 255 });
		lastY += offsetY;
	}

	renderEngine->EndScene();
	return true;
}