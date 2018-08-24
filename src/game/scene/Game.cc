#include "Game.h"
#include <game/IGame.h>
#include <engine/core/SDLResourceManager.h>
#include <SDL_opengl.h>
#include <SDL_opengl.h>
#if defined(_WIN32)
#include <GL/GLU.h>
#else
#include <OpenGL/glu.h>
#endif
#include <algorithm>
#include <chrono>

using namespace galaxy::demo;
using namespace galaxy::demo::scene;
using namespace galaxy::demo::networking;

namespace
{

	constexpr uint32_t MIN_PLAYERS_TO_PLAY = 2;

}

Game::Game(const IGamePtr& _game)
	: GameState(_game)
{
}

bool Game::Init()
{
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glViewport(0, 0, 1280, 720);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)1280 / (GLfloat)720, 1.0f, 150.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (!gameManager.Init())
		return false;

	camera.PositionCamera(-10, 5.f, 6, 0, 1.5f, 0, 0, 1, 0);
	startGameTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return true;
}

bool Game::Release()
{
	gameManager.Release();
	glDisable(GL_DEPTH_TEST);
	return true;
}

void Game::OnMouseDown(std::uint32_t x, std::uint32_t y)
{
}

void Game::OnMouseMotion(std::uint32_t x, std::uint32_t y)
{
}

void Game::OnKeyDown(SDL_Keysym key)
{
	switch (key.sym)
	{
		case SDLK_ESCAPE:
			game->SetClient(nullptr);
			game->SetServer(nullptr);
			game->SetLobby(nullptr);
			game->SetGameState(GameState::State::START_MENU);
			break;

		case SDLK_UP:
			camera.MoveCamera(0.125);
			break;

		case SDLK_DOWN:
			camera.MoveCamera(-0.125);
			break;

		case SDLK_LEFT:
		{
			const auto& client = game->GetClient();
			if (client)
			{
				const auto& gameManager = game->GetGameManager();
				if (gameManager.GetClientState() == GameManager::ClientState::GAME)
				{
					const auto& localPlayer = gameManager.GetLocalPlayer();
					const auto& playerDirection = localPlayer->GetDirection();
					glm::vec2 newPlayerDirection;

					if (playerDirection.x > 0)
						newPlayerDirection = glm::vec2(0, -1);
					else if (playerDirection.x < 0)
						newPlayerDirection = glm::vec2(0, 1);
					else if (playerDirection.y > 0)
						newPlayerDirection = glm::vec2(1, 0);
					else if (playerDirection.y < 0)
						newPlayerDirection = glm::vec2(-1, 0);

					client->SendUpdateDirectionToServer(newPlayerDirection);
				}
			}
		}
		break;

		case SDLK_RIGHT:
		{
			const auto& client = game->GetClient();
			if (client)
			{
				const auto& gameManager = game->GetGameManager();
				if (gameManager.GetClientState() == GameManager::ClientState::GAME)
				{
					const auto& localPlayer = gameManager.GetLocalPlayer();
					const auto& playerDirection = localPlayer->GetDirection();
					glm::vec2 newPlayerDirection;

					if (playerDirection.x > 0)
						newPlayerDirection = glm::vec2(0, 1);
					else if (playerDirection.x < 0)
						newPlayerDirection = glm::vec2(0, -1);
					else if (playerDirection.y > 0)
						newPlayerDirection = glm::vec2(-1, 0);
					else if (playerDirection.y < 0)
						newPlayerDirection = glm::vec2(1, 0);

					client->SendUpdateDirectionToServer(newPlayerDirection);
				}
			}
		}
		break;

		default:
			break;
	}
}

void Game::SendGameResults() const
{
	game->GetServer()->SendGameResults(game->GetGameManager().GetPlayers(), std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() - startGameTime);
}

void Game::OnLobbyEvent(const LobbyEvent& lobbyEvent)
{
	if (lobbyEvent == LobbyEvent::LOBBY_MEMBER_LEFT
		&& game->GetLobby()->GetLobbyMembers().size() < MIN_PLAYERS_TO_PLAY)
	{
		auto& gameServer = game->GetServer();
		if (!gameServer)
			return;

		SendGameResults();
	}
}

bool Game::Update()
{
	const auto& server = game->GetServer();
	if (server)
	{
		switch (game->GetGameManager().GetServerState())
		{
			case GameManager::ServerState::GAME:
			{
				auto& gameLogic = server->GetGameLogic();

				gameLogic.Tick();
				server->SendGameTick(server->GetPlayers());

				if (gameLogic.IsWinnerDefined())
					SendGameResults();
				break;
			}
			default:
				break;
		}
	}

	const auto& client = game->GetClient();
	if (client)
	{
		switch (game->GetGameManager().GetClientState())
		{
			case GameManager::ClientState::GAME:
				if (game->GetLobby()->GetLobbyMembers().size() < MIN_PLAYERS_TO_PLAY)
				{
					game->SetGameState(GameState::State::GAME_RESULT);
				}
				return true;
				break;

			case GameManager::ClientState::RESULT:
				game->SetGameState(GameState::State::IN_LOBBY_MENU);
				break;

			default:
				break;
		}
	}

	return false;
}

bool Game::Display(const renderer::OGLRendererPtr& renderEngine)
{
	glPushMatrix();
	glLoadIdentity();

	const auto& gameManager = game->GetGameManager();
	const auto& players = gameManager.GetPlayers();
	const auto& localPlayer = std::find_if(std::begin(players), std::end(players), [](const PlayerPtr& p) {return p->GetGalaxyID() == galaxy::api::User()->GetGalaxyID(); });

	if (localPlayer != std::end(players))
	{
		if (!(*localPlayer)->IsAlive())
		{
			gluLookAt(camera.m_vPosition.x + 10, camera.m_vPosition.y + 20, camera.m_vPosition.z + 10,
				camera.m_vView.x, camera.m_vView.y, camera.m_vView.z,
				camera.m_vUpVector.x, camera.m_vUpVector.y, camera.m_vUpVector.z);
		}
		else
		{
			const auto& playerPosition = (*localPlayer)->GetPosition();
			gluLookAt(playerPosition.x + camera.m_vPosition.x, playerPosition.y + camera.m_vPosition.y, playerPosition.z + camera.m_vPosition.z,
				playerPosition.x, playerPosition.y, playerPosition.z,
				camera.m_vUpVector.x, camera.m_vUpVector.y, camera.m_vUpVector.z);
		}
	}

	renderEngine->Display3DGrid();

	for (const auto& player : gameManager.GetPlayers())
	{
		if (!player->IsAlive())
			continue;

		const auto& playerPosition = player->GetPosition();

		renderEngine->DisplayCube(playerPosition, player->GetColor());

		const auto& playerTail = player->GetTail();
		for (const auto& tailPart : playerTail)
		{
			renderEngine->DisplayCube(tailPart->GetPosition(), tailPart->GetColor());
		}
	}

	glPopMatrix();

	return true;
}
