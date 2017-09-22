#include "Gogtron.h"
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

using namespace gogtron;
using namespace gogtron::scene;
using namespace gogtron::networking;

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

GogTron::GogTron(const renderer::OGLRendererPtr& _renderEngine)
	: renderEngine(_renderEngine)
{
}

void GogTron::InitListeners()
{
	listeners.emplace_back(std::make_unique<AuthListener>(shared_from_this()));
	listeners.emplace_back(std::make_unique<UserStatsAndAchievementsRetrieveListener>(shared_from_this()));
	listeners.emplace_back(std::make_unique<AchievementChangeListener>(shared_from_this()));
	listeners.emplace_back(std::make_unique<StatsAndAchievementsStoreListener>(shared_from_this()));
	listeners.emplace_back(std::make_unique<LeaderboardsRetrieveListener>(shared_from_this()));
	listeners.emplace_back(std::make_unique<LeaderboardEntriesRetrieveListener>(shared_from_this()));
	listeners.emplace_back(std::make_unique<LeaderboardScoreUpdateListener>(shared_from_this()));
	listeners.emplace_back(std::make_unique<FileShareListener>(shared_from_this()));
	listeners.emplace_back(std::make_unique<SharedFileDownloadListener>(shared_from_this()));
	listeners.emplace_back(std::make_unique<GameJoinRequestedListener>(shared_from_this()));
}

bool GogTron::Init(int argc, char** argv)
{
	ParseCmdLineArgs(argc, argv);

	if (!InitFontTextures())
		return false;

	InitGalaxy();

	InitListeners();

	galaxy::api::User()->SignIn();

	SetGameState(GameState::State::START_MENU);
	return true;
}

bool GogTron::Release()
{
	ReleaseGalaxy();

	if (gameState)
		gameState->Release();

	core::SDLResourceManager::ReleaseInstance();

	return true;
}

bool GogTron::Update()
{
	float deltaTime = 0.0f;
	if (AnimateNextFrame(30, deltaTime))
	{
		CalculateFrameRate(deltaTime);

		if (gameState && gameState->Update() && !stateChanged)
			gameState->Display(renderEngine);

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

void GogTron::OnMouseDown(std::uint32_t x, std::uint32_t y)
{
	if (!gameState)
		return;

	gameState->OnMouseDown(x, y);
}

void GogTron::OnMouseMotion(std::uint32_t x, std::uint32_t y)
{
	if (!gameState)
		return;

	gameState->OnMouseMotion(x, y);
}

void GogTron::OnKeyDown(SDL_Keysym key)
{
	if (!gameState)
		return;

	gameState->OnKeyDown(key);
}

void GogTron::OnLobbyEvent(const LobbyEvent& lobbyEvent)
{
	if (!gameState)
		return;

	gameState->OnLobbyEvent(lobbyEvent);
}

bool GogTron::SetGameState(const GameState::State& state)
{
	if (gameState)
		gameState->Release();

	currentGameState = state;
	stateChanged = true;
	return true;
}

bool GogTron::AnimateNextFrame(std::uint32_t fps, float& globalDeltaTime)
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

void GogTron::CalculateFrameRate(float& globalDeltaTime)
{
	static int framesPerSecond = 0;				// This will store our fps
	static float fpsTime = 0.0f;				// Amount of elapsed time until we update the FPS count
	char strFrameRate[50] = {0};				// We will store the string here for the window title

	// Increase the fps elapsed time
	// g_DT is the delta time, that is, the time between the last frame and the current frame
	fpsTime += globalDeltaTime;

	// Check to see if our time is greater than 1 second, if it is, then
	// record the frames per second.
	if (fpsTime > 1.0f)
	{
		// Reset the fpsTime
		fpsTime = 0.0f;

		// Copy the frames per second into a string to display in the window title bar
		sprintf_s(strFrameRate, "Current Frames Per Second: %d", framesPerSecond);

		//std::cout << strFrameRate << std::endl;

		// Reset the frames per second
		framesPerSecond = 0;
	}
	else
	{
		// Increase the frame counter
		++framesPerSecond;
	}
}

bool GogTron::InitFontTextures()
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

void GogTron::InitGalaxy()
{

	try
	{
		galaxy::api::Init(CLIENT_ID, CLIENT_SECRET);

		galaxy::api::IsFullyInitialized = true;

		InitListeners();

	}
	catch (const galaxy::api::IError& /*error*/)
	{
		galaxy::api::IsFullyInitialized = false;
	}
}

bool GogTron::ReleaseGalaxy()
{
	listeners.clear();
	galaxy::api::Shutdown();
	return true;
}

GogTron::AuthListener::AuthListener(const std::shared_ptr<GogTron>& _game)
	: game(_game)
{
}

void GogTron::AuthListener::OnAuthSuccess()
{
	if (!connectionString.empty())
	{
		ILobbyPtr lobby = std::make_shared<Lobby>(game);
		game->SetLobby(lobby);
		if (!Lobby::ConnectToLobbyByConnectionString(connectionString))
			game->SetGameState(GameState::State::INIT_FAILED_VIEW);
	}

	auto&& userID = galaxy::api::User()->GetGalaxyID();
	game->GetGameplayData().SetUserAchievements(userID, AchievementsFactory::CreateDefaultAchievements());
	game->GetGameplayData().SetUserStatistics(userID, StatisticsFactory::CreateDefaultStatistics());
	game->GetGameplayData().SetLeaderboards(LeaderboardFactory::CreateDefaultLeaderboards());

	galaxy::api::Stats()->RequestUserStatsAndAchievements();
	galaxy::api::Stats()->RequestLeaderboards();
}

void GogTron::AuthListener::OnAuthFailure(FailureReason reason)
{
	// TODO: implement notification for "Could not sign in"
	// TODO: implement reconnect button
}

void GogTron::AuthListener::OnAuthLost()
{
	// TODO: implement notification for "Auth lost"
	// TODO: implement reconnect button

	auto gameState = game->GetGameState();
	if (gameState == GameState::State::LOBBY_MENU
		|| gameState == GameState::State::JOIN_LOBBY_MENU
		|| gameState == GameState::State::IN_GAME)
		game->SetGameState(GameState::State::START_MENU);
}

GogTron::GameJoinRequestedListener::GameJoinRequestedListener(const std::shared_ptr<GogTron>& _game)
	: game(_game)
{
}

void GogTron::GameJoinRequestedListener::OnGameJoinRequested(galaxy::api::GalaxyID userID, const char* connectionString)
{
	ILobbyPtr lobby = std::make_shared<Lobby>(game);
	game->SetLobby(lobby);
	Lobby::ConnectToLobbyByConnectionString(connectionString);
}

GogTron::UserStatsAndAchievementsRetrieveListener::UserStatsAndAchievementsRetrieveListener(const std::shared_ptr<GogTron>& _game)
	: game(_game)
{
}

void GogTron::UserStatsAndAchievementsRetrieveListener::OnUserStatsAndAchievementsRetrieveSuccess(galaxy::api::GalaxyID userID)
{
	try
	{
		Achievements& userAchievements = game->GetGameplayData().GetUserAchievements(userID);
		Achievements::iterator userAchievementsIter(userAchievements.begin()), userAchievementsIterEnd(userAchievements.end());
		for (; userAchievementsIter != userAchievementsIterEnd; ++userAchievementsIter)
		{
			galaxy::api::Stats()->GetAchievement(userAchievementsIter->second.name.c_str(), userAchievementsIter->second.unlocked, userAchievementsIter->second.unlockedTime, userID);
			userAchievementsIter->second.description = galaxy::api::Stats()->GetAchievementDescription(userAchievementsIter->second.name.c_str());
			userAchievementsIter->second.displayName = galaxy::api::Stats()->GetAchievementDisplayName(userAchievementsIter->second.name.c_str());
			userAchievementsIter->second.visible = galaxy::api::Stats()->IsAchievementVisible(userAchievementsIter->second.name.c_str());
		}

		Statistics& usersStatistics = game->GetGameplayData().GetUserStatistics(userID);
		Statistics::iterator userStatisticsIter(usersStatistics.begin()), userStatisticsIterEnd(usersStatistics.end());
		for (; userStatisticsIter != userStatisticsIterEnd; ++userStatisticsIter)
		{
			switch (userStatisticsIter->second.GetType())
			{
				case Statistic::INT:
				{
					int32_t value = galaxy::api::Stats()->GetStatInt(userStatisticsIter->second.GetName().c_str(), userID);
					userStatisticsIter->second.SetInt(value);
					break;
				}

				case Statistic::FLOAT:
				{
					float value = galaxy::api::Stats()->GetStatFloat(userStatisticsIter->second.GetName().c_str(), userID);
					userStatisticsIter->second.SetFloat(value);
					break;
				}

				case Statistic::AVG:
				{
					float value = galaxy::api::Stats()->GetStatFloat(userStatisticsIter->second.GetName().c_str(), userID);
					userStatisticsIter->second.SetAvg(value);
					break;
				}

				default:
					assert(0 && "Unknown statistic type");
			}
		}

		game->GetGameplayData().SetStatsAndAchievementStatus(true);
	}
	catch (const galaxy::api::IError& error)
	{
		fprintf(stderr, "Error occurred during OnUserStatsAndAchievementsRetrieveSuccess execution, error=%s\n", error.GetMsg());
	}
}

void GogTron::UserStatsAndAchievementsRetrieveListener::OnUserStatsAndAchievementsRetrieveFailure(galaxy::api::GalaxyID userID, FailureReason failureReason)
{
}

GogTron::AchievementChangeListener::AchievementChangeListener(const std::shared_ptr<GogTron>& _game)
	: game(_game)
{
}

void GogTron::AchievementChangeListener::OnAchievementUnlocked(const char* name)
{
	try
	{
		auto&& userID = galaxy::api::User()->GetGalaxyID();
		Achievements& userAchievements = game->GetGameplayData().GetUserAchievements(userID);
		Achievements::iterator userAchievement = userAchievements.find(name);
		galaxy::api::Stats()->GetAchievement(name, userAchievement->second.unlocked, userAchievement->second.unlockedTime, userID);
	}
	catch (const galaxy::api::IError& error)
	{
		fprintf(stderr, "Error occurred during OnAchievementUnlocked execution, error=%s\n", error.GetMsg());
	}
}

GogTron::StatsAndAchievementsStoreListener::StatsAndAchievementsStoreListener(const std::shared_ptr<GogTron>& _game)
	: game(_game)
{
}

void GogTron::StatsAndAchievementsStoreListener::OnUserStatsAndAchievementsStoreSuccess()
{
	try
	{
		auto&& userID = galaxy::api::User()->GetGalaxyID();
		Statistics& userStatistics = game->GetGameplayData().GetUserStatistics(userID);
		Statistics::iterator userStatisticsIter(userStatistics.begin()), userStatisticsIterEnd(userStatistics.end());
		for (; userStatisticsIter != userStatisticsIterEnd; ++userStatisticsIter)
		{
			switch (userStatisticsIter->second.GetType())
			{
				case Statistic::INT:
				{
					int32_t value = galaxy::api::Stats()->GetStatInt(userStatisticsIter->second.GetName().c_str(), userID);
					userStatisticsIter->second.SetInt(value);
					break;
				}

				case Statistic::FLOAT:
				{
					float value = galaxy::api::Stats()->GetStatFloat(userStatisticsIter->second.GetName().c_str(), userID);
					userStatisticsIter->second.SetFloat(value);
					break;
				}

				case Statistic::AVG:
				{
					float value = galaxy::api::Stats()->GetStatFloat(userStatisticsIter->second.GetName().c_str(), userID);
					userStatisticsIter->second.SetAvg(value);
					break;
				}

				default:
					assert(0 && "Unknown statistic type");
			}
		}

		Achievements& userAchievements = game->GetGameplayData().GetUserAchievements(userID);
		Achievements::iterator userAchievementsIter(userAchievements.begin()), userAchievementsIterEnd(userAchievements.end());
		for (; userAchievementsIter != userAchievementsIterEnd; ++userAchievementsIter)
		{
			galaxy::api::Stats()->GetAchievement(userAchievementsIter->second.name.c_str(), userAchievementsIter->second.unlocked, userAchievementsIter->second.unlockedTime, userID);
			userAchievementsIter->second.description = galaxy::api::Stats()->GetAchievementDescription(userAchievementsIter->second.name.c_str());
			userAchievementsIter->second.displayName = galaxy::api::Stats()->GetAchievementDisplayName(userAchievementsIter->second.name.c_str());
			userAchievementsIter->second.visible = galaxy::api::Stats()->IsAchievementVisible(userAchievementsIter->second.name.c_str());
		}
	}
	catch (const galaxy::api::IError& error)
	{
		fprintf(stderr, "Error occurred during OnUserStatsAndAchievementsStoreSuccess execution, error=%s\n", error.GetMsg());
	}
}

void GogTron::StatsAndAchievementsStoreListener::OnUserStatsAndAchievementsStoreFailure(FailureReason failureReason)
{
}

GogTron::LeaderboardsRetrieveListener::LeaderboardsRetrieveListener(const std::shared_ptr<GogTron>& _game)
	: game(_game)
{
}

void GogTron::LeaderboardsRetrieveListener::OnLeaderboardsRetrieveSuccess()
{
	game->GetGameplayData().SetLeaderboardsStatus(true);
}

void GogTron::LeaderboardsRetrieveListener::OnLeaderboardsRetrieveFailure(FailureReason failureReason)
{
}

GogTron::LeaderboardEntriesRetrieveListener::LeaderboardEntriesRetrieveListener(const std::shared_ptr<GogTron>& _game)
	: game(_game)
{
}

void GogTron::LeaderboardEntriesRetrieveListener::OnLeaderboardEntriesRetrieveSuccess(const char* name, uint32_t entryCount)
{
	try
	{
		Leaderboard& leaderboard = game->GetGameplayData().GetLeaderboard(name);

		leaderboard.displayName = galaxy::api::Stats()->GetLeaderboardDisplayName(name);
		leaderboard.sortMethod = galaxy::api::Stats()->GetLeaderboardSortMethod(name);
		leaderboard.displayType = galaxy::api::Stats()->GetLeaderboardDisplayType(name);

		Leaderboard::Entries entires;
		for (uint32_t i = 0; i < entryCount; ++i)
		{
			Leaderboard::Entry entry;
			galaxy::api::Stats()->GetRequestedLeaderboardEntry(i, entry.rank, entry.score, entry.userID);
			const char* userName = galaxy::api::Friends()->GetFriendPersonaName(entry.userID);
			fprintf(stdout, "[%u] user name=%s, userID=%llu, rank=%u, score=%d\n", i, userName, entry.userID.ToUint64(), entry.rank, entry.rank);
			entires.push_back(entry);
		}

		leaderboard.entries.swap(entires);
	}
	catch (const galaxy::api::IError& error)
	{
		fprintf(stderr, "Error occurred during OnLeaderboardEntriesRetrieveSuccess execution, error=%s", error.GetMsg());
	}
}

void GogTron::LeaderboardEntriesRetrieveListener::OnLeaderboardEntriesRetrieveFailure(const char* name, FailureReason failureReason)
{
}

GogTron::LeaderboardScoreUpdateListener::LeaderboardScoreUpdateListener(const std::shared_ptr<GogTron>& _game)
	: game(_game)
{
}

void GogTron::LeaderboardScoreUpdateListener::OnLeaderboardScoreUpdateSuccess(const char* name, int32_t score, uint32_t oldRank, uint32_t newRank)
{
	auto&& userID = galaxy::api::User()->GetGalaxyID();
	Leaderboard& leaderboard = game->GetGameplayData().GetLeaderboard(name);
	Leaderboard::Entries::iterator it(leaderboard.entries.begin()), itEnd(leaderboard.entries.end());
	for (; it < itEnd; ++it)
	{
		if (it->userID == userID)
		{
			it->score = score;
			it->rank = newRank;
			return;
		}
	}

	Leaderboard::Entry entry;
	entry.rank = newRank;
	entry.score = score;
	entry.userID = userID;
	leaderboard.entries.push_back(entry);
}

void GogTron::LeaderboardScoreUpdateListener::OnLeaderboardScoreUpdateFailure(const char* name, int32_t score, FailureReason failureReason)
{
}

gogtron::GogTron::FileShareListener::FileShareListener(const std::shared_ptr<GogTron>& _game)
	: game(_game)
{
}

void gogtron::GogTron::FileShareListener::OnFileShareSuccess(const char* fileName, galaxy::api::SharedFileID sharedFileID)
{
	game->SetStorageSynchronizationStatus(IGame::FileSharingStatus::SHARED);
	//TODO: implement file sharing
	throw std::logic_error("File sharing not fully implemented");
}

void gogtron::GogTron::FileShareListener::OnFileShareFailure(const char* fileName, FailureReason failureReason)
{
	game->SetStorageSynchronizationStatus(IGame::FileSharingStatus::FAILED);
	//TODO: implement file sharing
	throw std::logic_error("File sharing not fully implemented");
}

gogtron::GogTron::SharedFileDownloadListener::SharedFileDownloadListener(const std::shared_ptr<GogTron>& _game)
	: game(_game)
{
}

void gogtron::GogTron::SharedFileDownloadListener::OnSharedFileDownloadSuccess(galaxy::api::SharedFileID sharedFileID, const char* fileName)
{
	game->SetStorageSynchronizationStatus(IGame::FileSharingStatus::DOWNLOADED);
	//TODO: implement file sharing
	throw std::logic_error("File sharing not fully implemented");
}

void gogtron::GogTron::SharedFileDownloadListener::OnSharedFileDownloadFailure(galaxy::api::SharedFileID sharedFileID, FailureReason failureReason)
{
	game->SetStorageSynchronizationStatus(IGame::FileSharingStatus::FAILED);
	//TODO: implement file sharing
	throw std::logic_error("File sharing not fully implemented");
}