#include "GalaxyDemo.h"
#include <game/networking/Lobby.h>
#include <game/scene/InitFailedView.h>
#include <game/scene/SinglePlayerView.h>
#include <game/scene/StartMenu.h>
#include <game/scene/StatsView.h>
#include <game/scene/LeaderboardsView.h>
#include <game/scene/CloudStorageView.h>
#include <game/scene/LobbyMenu.h>
#include <game/scene/JoinLobbyMenu.h>
#include <game/scene/InsideLobbyMenu.h>
#include <game/scene/Game.h>
#include <game/scene/Results.h>
#include <engine/core/SDLResourceManager.h>

namespace galaxy
{
	namespace api
	{
		bool IsFullyInitialized = false;
	}
}

using namespace galaxy::demo;
using namespace galaxy::demo::scene;
using namespace galaxy::demo::networking;

namespace
{

	static const char* CLIENT_ID = "48500786244777727";
	static const char* CLIENT_SECRET = "8573705eedaf4808fb704aeecc3763248849238b316e42f9e276b0bf40d01efe";

	static bool stateChanged = false;
	static std::string connectionString = "";

	static bool ParseCmdLineArgs(int argc, char** argv)
	{
		const std::string connectionStringPrefix = "+connect_";
		for (int i = 0; i < argc; ++i)
		{
			std::string cmdArg = argv[i];
			if (cmdArg.find(connectionStringPrefix) != std::string::npos)
			{
				connectionString = cmdArg;
				return true;
			}
		}
		return false;
	}

}

GalaxyDemo::GalaxyDemo(const renderer::OGLRendererPtr& _renderEngine)
	: renderEngine(_renderEngine)
{
}

void GalaxyDemo::InitListeners()
{
	listeners.emplace_back(std::make_unique<AuthListener>(shared_from_this()));
	listeners.emplace_back(std::make_unique<GameJoinRequestedListener>(shared_from_this()));
}

bool GalaxyDemo::Init(int argc, char** argv)
{
	ParseCmdLineArgs(argc, argv);

	if (!InitFontTextures())
		return false;

	InitGalaxy();

	galaxy::api::User()->SignIn();

	SetGameState(GameState::State::START_MENU);
	return true;
}

bool GalaxyDemo::Release()
{
	ReleaseGalaxy();

	if (gameState)
		gameState->Release();

	core::SDLResourceManager::ReleaseInstance();

	return true;
}

bool GalaxyDemo::Update()
{
	float deltaTime = 0.0f;
	if (AnimateNextFrame(30, deltaTime))
	{
		CalculateFrameRate(deltaTime);

		if (gameState && gameState->Update() && !stateChanged)
			gameState->Render(renderEngine);

		galaxy::api::ProcessData();
	}
	else
	{
		galaxy::api::ProcessData();
		SDL_Delay(1);
	}

	if (stateChanged)
	{
		switch (currentGameState)
		{
			case GameState::State::INIT_FAILED_VIEW:
				gameState = std::make_shared<InitFailedView>(shared_from_this());
				break;

			case GameState::State::START_MENU:
				gameState = std::make_shared<StartMenu>(shared_from_this());
				break;

			case GameState::State::SINGLE_PLAYER_VIEW:
				gameState = std::make_shared<SinglePlayerView>(shared_from_this());
				break;

			case GameState::State::STATS_VIEW:
				gameState = std::make_shared<StatsView>(shared_from_this());
				break;

			case GameState::State::LEADERBOARDS_VIEW:
				gameState = std::make_shared<LeaderboardsView>(shared_from_this());
				break;

			case GameState::State::CLOUD_STORAGE_VIEW:
				gameState = std::make_shared<CloudStorageView>(shared_from_this());
				break;

			case GameState::State::LOBBY_MENU:
				gameState = std::make_shared<LobbyMenu>(shared_from_this());
				break;

			case GameState::State::JOIN_LOBBY_MENU:
				gameState = std::make_shared<JoinLobbyMenu>(shared_from_this());
				break;

			case GameState::State::IN_LOBBY_MENU:
				gameState = std::make_shared<InsideLobbyMenu>(shared_from_this());
				break;

			case GameState::State::IN_GAME:
				gameState = std::make_shared<Game>(shared_from_this());
				break;

			case GameState::State::GAME_RESULT:
				gameState = std::make_shared<Results>(shared_from_this());
				break;
		}

		if (!gameState->Init())
			return false;

		stateChanged = false;
	}

	return true;
}

void GalaxyDemo::OnMouseDown(std::uint32_t x, std::uint32_t y)
{
	if (!gameState)
		return;

	gameState->OnMouseDown(x, y);
}

void GalaxyDemo::OnMouseMotion(std::uint32_t x, std::uint32_t y)
{
	if (!gameState)
		return;

	gameState->OnMouseMotion(x, y);
}

void GalaxyDemo::OnKeyDown(SDL_Keysym key)
{
	if (!gameState)
		return;

	gameState->OnKeyDown(key);
}

void GalaxyDemo::OnLobbyEvent(const LobbyEvent& lobbyEvent)
{
	if (!gameState)
		return;

	gameState->OnLobbyEvent(lobbyEvent);
}

bool GalaxyDemo::SetGameState(const GameState::State& state)
{
	if (gameState)
		gameState->Release();

	currentGameState = state;
	stateChanged = true;
	return true;
}

bool GalaxyDemo::AnimateNextFrame(std::uint32_t fps, float& globalDeltaTime)
{
	static float lastTime = SDL_GetTicks() * 0.001f;
	static float elapsedTime = 0.0f;

	float currentTime = SDL_GetTicks() * 0.001f; // Get the time (milliseconds = seconds * .001)
	float deltaTime = currentTime - lastTime; // Get the slice of time
	float desiredFPS = 1.0f / fps; // Store 1 / desiredFrameRate

	elapsedTime += deltaTime; // Add to the elapsed time
	lastTime = currentTime; // Update lastTime

	// Check if the time since we last checked is greater than our desiredFPS
	if (elapsedTime > desiredFPS)
	{
		globalDeltaTime = desiredFPS; // Set the delta time
		elapsedTime -= desiredFPS; // Adjust our elapsed time

		// Return true, to animate the next frame of animation
		return true;
	}

	// We don't animate right now.
	return false;
}

void GalaxyDemo::CalculateFrameRate(float& globalDeltaTime)
{
	static int framesPerSecond = 0;				// This will store our fps
	static float fpsTime = 0.0f;				// Amount of elapsed time until we update the FPS count

	// Increase the fps elapsed time
	// g_DT is the delta time, that is, the time between the last frame and the current frame
	fpsTime += globalDeltaTime;

	// Check to see if our time is greater than 1 second, if it is, then
	// record the frames per second.
	if (fpsTime > 1.0f)
	{
		// Reset the fpsTime
		fpsTime = 0.0f;

		// TODO: Display in-game FPS

		// Reset the frames per second
		framesPerSecond = 0;
	}
	else
	{
		// Increase the frame counter
		++framesPerSecond;
	}
}

bool GalaxyDemo::InitFontTextures()
{
	core::SDLResourceManager& sdlResourceManager = core::SDLResourceManager::GetInstance();

	if (!sdlResourceManager.LoadTexture("res//images//button.png", "button"))
		return false;

	if (!sdlResourceManager.LoadTexture("res//images//selectedbutton.png", "selectedbutton"))
		return false;

	if (!sdlResourceManager.LoadFont("res//fonts//FreeSans.ttf", "FreeSans"))
		return false;

	return true;
}

void GalaxyDemo::InitGalaxy()
{
	try
	{
		galaxy::api::Init({CLIENT_ID, CLIENT_SECRET});

		galaxy::api::IsFullyInitialized = true;

		InitListeners();
	}
	catch (const galaxy::api::IError& /*error*/)
	{
		galaxy::api::IsFullyInitialized = false;
	}
}

bool GalaxyDemo::ReleaseGalaxy()
{
	// Destroy all listeners before shutting down GalaxySDK
	listeners.clear();

	galaxy::api::Shutdown();
	return true;
}

GalaxyDemo::AuthListener::AuthListener(const std::shared_ptr<GalaxyDemo>& _game)
	: game(_game)
{
}

void GalaxyDemo::AuthListener::OnAuthSuccess()
{
	if (!connectionString.empty())
	{
		ILobbyPtr lobby = std::make_shared<Lobby>(game);
		game->SetLobby(lobby);
		if (!Lobby::ConnectToLobbyByConnectionString(connectionString))
			game->SetGameState(GameState::State::INIT_FAILED_VIEW);
	}

	game->GetGameplayData().Init();
}

void GalaxyDemo::AuthListener::OnAuthFailure(FailureReason reason)
{
	// TODO: implement notification for "Could not sign in"
	// TODO: implement reconnect button
}

void GalaxyDemo::AuthListener::OnAuthLost()
{
	// TODO: implement notification for "Auth lost"
	// TODO: implement reconnect button

	auto gameState = game->GetGameState();
	if (gameState == GameState::State::LOBBY_MENU
		|| gameState == GameState::State::JOIN_LOBBY_MENU
		|| gameState == GameState::State::IN_GAME)
		game->SetGameState(GameState::State::START_MENU);
}

GalaxyDemo::GameJoinRequestedListener::GameJoinRequestedListener(const std::shared_ptr<GalaxyDemo>& _game)
	: game(_game)
{
}

void GalaxyDemo::GameJoinRequestedListener::OnGameJoinRequested(galaxy::api::GalaxyID userID, const char* connectionString)
{
	ILobbyPtr lobby = std::make_shared<Lobby>(game);
	game->SetLobby(lobby);
	Lobby::ConnectToLobbyByConnectionString(connectionString);
}
