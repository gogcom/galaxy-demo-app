#include "Listeners.h"
#include <cassert>

namespace game
{

	LobbyEnteredListener::LobbyEnteredListener(GameContext& _gameContext)
		: GameContextListener(_gameContext)
	{
	}

	void LobbyEnteredListener::OnLobbyEntered(const galaxy::api::GalaxyID& lobbyID, galaxy::api::LobbyEnterResult result)
	{
		switch (result)
		{
		case galaxy::api::LOBBY_ENTER_RESULT_SUCCESS:
			fprintf(stdout, "Lobby %llu entered\n", lobbyID.ToUint64());
			gameContext.SetGameState(GameContext::IN_LOBBY);
			gameContext.SetLobbyID(lobbyID);
			gameContext.PrintGameControls();
			break;

		case galaxy::api::LOBBY_ENTER_RESULT_LOBBY_IS_FULL:
			fprintf(stdout, "Lobby %llu is full\n", lobbyID.ToUint64());
			gameContext.SetGameState(GameContext::QUIT);
			break;

		default:
			fprintf(stdout, "Failed to join lobby %llu\n", lobbyID.ToUint64());
			gameContext.SetGameState(GameContext::QUIT);
		}
	}

	void LobbyMemberStateListener::OnLobbyMemberStateChanged(const galaxy::api::GalaxyID& lobbyID, const galaxy::api::GalaxyID& memberID, galaxy::api::LobbyMemberStateChange memberStateChange)
	{
		if (memberStateChange == galaxy::api::LOBBY_MEMBER_STATE_CHANGED_ENTERED)
		{
			fprintf(stdout, "User %llu has joined lobby %llu\n", memberID.ToUint64(), lobbyID.ToUint64());
		}
		else
		{
			fprintf(stdout, "User %llu has left lobby %llu\n", memberID.ToUint64(), lobbyID.ToUint64());
		}
	}

	LobbyCreatedListener::LobbyCreatedListener(GameContext& _gameContext)
		: GameContextListener(_gameContext)
	{
	}

	void LobbyCreatedListener::OnLobbyCreated(const galaxy::api::GalaxyID& lobbyID, galaxy::api::LobbyCreateResult result)
	{
		if (result == galaxy::api::LOBBY_CREATE_RESULT_SUCCESS)
		{
			fprintf(stdout, "Lobby %llu created\n", lobbyID.ToUint64());
		}
		else
		{
			fprintf(stdout, "Failed to create a lobby\n");
			gameContext.SetGameState(GameContext::QUIT);
		}
	}

	LobbyListListener::LobbyListListener(GameContext& _gameContext)
		: GameContextListener(_gameContext)
	{
	}

	void LobbyListListener::OnLobbyList(uint32_t lobbyCount, bool ioFailure)
	{
		if (ioFailure)
		{
			fprintf(stdout, "Failed to retrieve lobby list\n");
			gameContext.SetGameState(GameContext::QUIT);
			return;
		}

		try
		{
			if (!lobbyCount)
			{
				// there are no lobbies available so we create a new one
				fprintf(stdout, "No lobbies available, creating a new one...\n");
				const uint32_t maxLobbyMemberCount = 4;
				galaxy::api::Matchmaking()->CreateLobby(galaxy::api::LOBBY_TYPE_PUBLIC, maxLobbyMemberCount);
			}
			else
			{
				// there is a lobby so we join it
				galaxy::api::GalaxyID lobbyID = galaxy::api::Matchmaking()->GetLobbyByIndex(0);
				fprintf(stdout, "Joining lobby %llu...\n", lobbyID.ToUint64());
				galaxy::api::Matchmaking()->JoinLobby(lobbyID);
			}
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Error occurred during OnLobbyList execution, error=%s\n", error.GetMsg());
		}
	}

	NetworkingListener::NetworkingListener(GameContext& _gameContext)
		: GameContextListener(_gameContext)
	{
	}

	void NetworkingListener::OnP2PPacketAvailable(uint32_t msgSize, uint8_t channel)
	{
		assert(gameContext == GameContext::IN_LOBBY);

		try
		{
			galaxy::api::GalaxyID sender;
			char incommingPacket[2048];
			if (galaxy::api::Networking()->PeekP2PPacket(incommingPacket, sizeof(incommingPacket), &msgSize, sender, 0))
			{
				incommingPacket[msgSize] = '\0';
				fprintf(stdout, "Received p2p packet from %llu Packet content:\n%s\n", sender.ToUint64(), incommingPacket);
			}
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Failed to call ReadP2PPacket, error=%s\n", error.GetMsg());
		}
	}

	ServerNetworkingListener::ServerNetworkingListener(GameContext& _gameContext)
		: GameContextListener(_gameContext)
	{
	}

	void ServerNetworkingListener::OnServerP2PPacketAvailable(uint32_t msgSize, uint8_t channel)
	{
		assert(gameContext == GameContext::IN_LOBBY);
		assert(IsCurrentUserLobbyOwner());

		try
		{
			galaxy::api::GalaxyID sender;
			char incommingPacket[2048];
			if (galaxy::api::ServerNetworking()->PeekP2PPacket(incommingPacket, sizeof(incommingPacket), &msgSize, sender, 0))
			{
				incommingPacket[msgSize] = '\0';
				fprintf(stdout, "Server (lobby owner) has received p2p packet from %llu Packet content:\n%s\n", sender.ToUint64(), incommingPacket);
			}
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Failed to call ReadP2PPacket, error=%s\n", error.GetMsg());
		}
	}

	bool ServerNetworkingListener::IsCurrentUserLobbyOwner() const
	{
		try
		{
			return gameContext.GetUserID() == galaxy::api::Matchmaking()->GetLobbyOwner(gameContext.GetLobbyID());
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Failed to call GetLobbyOwner, error=%s\n", error.GetMsg());
			return false;
		}
	}

	AuthListener::AuthListener(GameContext& _gameContext)
		: GameContextListener(_gameContext)
	{
	}

	void AuthListener::OnAuthSuccess()
	{
		try
		{
			if (!galaxy::api::User()->IsLoggedOn())
			{
				fprintf(stdout, "Successfully signed in, but in the offline mode\n");
				// the game should support it and continue working with some Galaxy features unavailable.
				gameContext.SetGameState(GameContext::QUIT);
				return;
			}

			fprintf(stdout, "Successfully signed in as %s\n", galaxy::api::Friends()->GetPersonaName());

			// after successfully signing in request a lobby list to check if there are any lobbies
			fprintf(stdout, "Requesting lobby list...\n");
			galaxy::api::Matchmaking()->RequestLobbyList();

			gameContext.SetUserID(galaxy::api::User()->GetGalaxyID());
			gameContext.GetGameplayData().SetUserAchievements(gameContext.GetUserID(), AchievementsFactory::CreateDefaultAchievements());
			gameContext.GetGameplayData().SetUserStatistics(gameContext.GetUserID(), StatisticsFactory::CreateDefaultStatistics());
			gameContext.GetGameplayData().SetLeaderboards(LeaderboardFactory::CreateDefaultLeaderboards());
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Error occurred during OnAuthSuccess execution, error=%s\n", error.GetMsg());
		}
	}

	void AuthListener::OnAuthFailure(FailureReason reason)
	{
		fprintf(stdout, "Failed to sign in; reason %u\n", reason);
		gameContext.SetGameState(GameContext::QUIT);
	}

	void AuthListener::OnAuthLost()
	{
		// we failed to refresh the authorization
		fprintf(stdout, "Authorization lost\n");
		gameContext.SetGameState(GameContext::QUIT);
	}

	void OperationalStateChangeListener::OnOperationalStateChanged(uint32_t operationalState)
	{
		fprintf(stdout, "Received notification for the event of a change of the operational state, operationalState=%u\n", operationalState);

		if (operationalState & OperationalStateChangeListener::OPERATIONAL_STATE_SIGNED_IN)
			fprintf(stdout, "User is signed in\n");
		else
			fprintf(stdout, "User is not signed in\n");

		if (operationalState & OperationalStateChangeListener::OPERATIONAL_STATE_LOGGED_ON)
			fprintf(stdout, "User is logged on\n");
		else
			fprintf(stdout, "User is not logged on\n");
	}

	void PersonaDataChangedListener::OnPersonaDataChanged(galaxy::api::GalaxyID userID, uint32_t personaStateChange)
	{
		fprintf(stdout, "Received notification for the event of changing persona data: userID=%llu, personaStateChange=%u\n", userID.ToUint64(), personaStateChange);
		// it is now possible to retrieve friend's data by calling methods
		// galaxy::api::IFriends::GetFriendPersonaName() or galaxy::api::IFriends::GetFriendAvatarUrl()
	}

	void LobbyMessageListener::OnLobbyMessageReceived(const galaxy::api::GalaxyID& lobbyID, const galaxy::api::GalaxyID& senderID, uint32_t messageID, uint32_t messageLength)
	{
		try
		{
			galaxy::api::GalaxyID sender;
			char* buffer = new char[messageLength + 1];
			uint32_t lengthOfMessage = galaxy::api::Matchmaking()->GetLobbyMessage(lobbyID, messageID, sender, buffer, messageLength);
			buffer[lengthOfMessage] = '\0';
			fprintf(stdout, "Received message %u from %llu in lobby %llu; message content:\n%s\n", messageID, sender.ToUint64(), lobbyID.ToUint64(), buffer);
			delete[] buffer;
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Error occurred during OnLobbyMessageReceived execution, error=%s\n", error.GetMsg());
		}
	}

	void LobbyDataListener::OnLobbyDataUpdated(const galaxy::api::GalaxyID& lobbyID, const galaxy::api::GalaxyID& memberID)
	{
		try
		{
			if (memberID.IsValid())
			{
				fprintf(stdout, "Member %llu of lobby %llu has changed some of his member data:\n", memberID.ToUint64(), lobbyID.ToUint64());

				for (uint32_t i = 0; i < galaxy::api::Matchmaking()->GetLobbyMemberDataCount(lobbyID, memberID); ++i)
				{
					char key[2048];
					char value[2048];
					if (galaxy::api::Matchmaking()->GetLobbyMemberDataByIndex(lobbyID, memberID, i, key, sizeof(key), value, sizeof(value)))
					{
						fprintf(stdout, "\t'%s' -> '%s'\n", key, value);
					}
				}
			}
			else
			{
				fprintf(stdout, "Lobby owner of lobby %llu has changed some of lobby data:\n", lobbyID.ToUint64());

				for (uint32_t i = 0; i < galaxy::api::Matchmaking()->GetLobbyDataCount(lobbyID); ++i)
				{
					char key[2048];
					char value[2048];
					if (galaxy::api::Matchmaking()->GetLobbyDataByIndex(lobbyID, i, key, sizeof(key), value, sizeof(value)))
					{
						fprintf(stdout, "\t'%s' -> '%s'\n", key, value);
					}
				}
			}
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Error occurred during OnLobbyDataUpdated execution, error=%s\n", error.GetMsg());
		}
	}

	void LobbyOwnerChangeListener::OnLobbyOwnerChanged(const galaxy::api::GalaxyID& lobbyID, const galaxy::api::GalaxyID& newOwnerID)
	{
		if (newOwnerID != galaxy::api::User()->GetGalaxyID())
		{
			fprintf(stdout, "The owner %llu of lobby has left; the new owner is %llu\n", lobbyID.ToUint64(), newOwnerID.ToUint64());
		}
		else
		{
			fprintf(stdout, "The owner %llu of lobby has left; the new owner is me\n", lobbyID.ToUint64());
		}
	}

	UserStatsAndAchievementsRetrieveListener::UserStatsAndAchievementsRetrieveListener(GameContext& _gameContext)
		: GameContextListener(_gameContext)
	{
	}

	void UserStatsAndAchievementsRetrieveListener::OnUserStatsAndAchievementsRetrieveSuccess(galaxy::api::GalaxyID userID)
	{
		fprintf(stdout, "Received stats and achievements for a user %llu\n", userID.ToUint64());

		try
		{
			Achievements& userAchievements = gameContext.GetGameplayData().GetUserAchievements(userID);
			Achievements::iterator userAchievementsIter(userAchievements.begin()), userAchievementsIterEnd(userAchievements.end());
			for (; userAchievementsIter != userAchievementsIterEnd; ++userAchievementsIter)
			{
				galaxy::api::Stats()->GetAchievement(userAchievementsIter->second.name.c_str(), userAchievementsIter->second.unlocked, userAchievementsIter->second.unlockedTime, userID);
				userAchievementsIter->second.description = galaxy::api::Stats()->GetAchievementDescription(userAchievementsIter->second.name.c_str());
				userAchievementsIter->second.displayName = galaxy::api::Stats()->GetAchievementDisplayName(userAchievementsIter->second.name.c_str());
				userAchievementsIter->second.visible = galaxy::api::Stats()->IsAchievementVisible(userAchievementsIter->second.name.c_str());
			}

			Statistics& usersStatistics = gameContext.GetGameplayData().GetUserStatistics(userID);
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

			gameContext.GetGameplayData().SetStatsAndAchievementStatus(true);
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Error occurred during OnUserStatsAndAchievementsRetrieveSuccess execution, error=%s\n", error.GetMsg());
		}
	}

	void UserStatsAndAchievementsRetrieveListener::OnUserStatsAndAchievementsRetrieveFailure(galaxy::api::GalaxyID userID, galaxy::api::IUserStatsAndAchievementsRetrieveListener::FailureReason failureReason)
	{
		fprintf(stdout, "Failed to receive stats and achievements for user %llu, reason %u\n", userID.ToUint64(), failureReason);
	}

	StatsAndAchievementsStoreListener::StatsAndAchievementsStoreListener(GameContext& _gameContext)
		: GameContextListener(_gameContext)
	{
	}

	void StatsAndAchievementsStoreListener::OnUserStatsAndAchievementsStoreSuccess()
	{
		fprintf(stdout, "Stored stats and achievements\n");

		try
		{
			Statistics& userStatistics = gameContext.GetGameplayData().GetUserStatistics(gameContext.GetUserID());
			Statistics::iterator userStatisticsIter(userStatistics.begin()), userStatisticsIterEnd(userStatistics.end());
			for (; userStatisticsIter != userStatisticsIterEnd; ++userStatisticsIter)
			{
				switch (userStatisticsIter->second.GetType())
				{
				case Statistic::INT:
					{
						int32_t value = galaxy::api::Stats()->GetStatInt(userStatisticsIter->second.GetName().c_str(), gameContext.GetUserID());
						userStatisticsIter->second.SetInt(value);
						break;
					}

				case Statistic::FLOAT:
					{
						float value = galaxy::api::Stats()->GetStatFloat(userStatisticsIter->second.GetName().c_str(), gameContext.GetUserID());
						userStatisticsIter->second.SetFloat(value);
						break;
					}

				case Statistic::AVG:
					{
						float value = galaxy::api::Stats()->GetStatFloat(userStatisticsIter->second.GetName().c_str(), gameContext.GetUserID());
						userStatisticsIter->second.SetAvg(value);
						break;
					}

				default:
					assert(0 && "Unknown statistic type");
				}
			}
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Error occurred during OnUserStatsAndAchievementsStoreSuccess execution, error=%s\n", error.GetMsg());
		}
	}

	void StatsAndAchievementsStoreListener::OnUserStatsAndAchievementsStoreFailure(galaxy::api::IStatsAndAchievementsStoreListener::FailureReason failureReason)
	{
		fprintf(stdout, "Failed to store stats and achievements, reason %u\n", failureReason);
	}

	AchievementChangeListener::AchievementChangeListener(GameContext& _gameContext)
		: GameContextListener(_gameContext)
	{
	}

	void AchievementChangeListener::OnAchievementUnlocked(const char* name)
	{
		fprintf(stdout, "Unlocked achievement: %s\n", name);

		try
		{
			Achievements& userAchievements = gameContext.GetGameplayData().GetUserAchievements(gameContext.GetUserID());
			Achievements::iterator userAchievement = userAchievements.find(name);
			galaxy::api::Stats()->GetAchievement(name, userAchievement->second.unlocked, userAchievement->second.unlockedTime, gameContext.GetUserID());
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Error occurred during OnAchievementUnlocked execution, error=%s\n", error.GetMsg());
		}
	}

	LeaderboardsRetrieveListener::LeaderboardsRetrieveListener(GameContext& _gameContext)
		: GameContextListener(_gameContext)
	{
	}

	void LeaderboardsRetrieveListener::OnLeaderboardsRetrieveSuccess()
	{
		fprintf(stdout, "Retrieve leaderboards\n");
		gameContext.GetGameplayData().SetLeaderboardsStatus(true);
	}

	void LeaderboardsRetrieveListener::OnLeaderboardsRetrieveFailure(galaxy::api::ILeaderboardsRetrieveListener::FailureReason failureReason)
	{
		fprintf(stdout, "Failed to retrieve leaderboards, reason %u\n", failureReason);
	}

	LeaderboardEntriesRetrieveListener::LeaderboardEntriesRetrieveListener(GameContext& _gameContext)
		: GameContextListener(_gameContext)
	{
	}

	void LeaderboardEntriesRetrieveListener::OnLeaderboardEntriesRetrieveSuccess(const char* name, uint32_t entryCount)
	{
		fprintf(stdout, "Retrieve leaderboard entries, leaderboard name %s, entry count %u\n", name, entryCount);

		try
		{
			Leaderboard& leaderboard = gameContext.GetGameplayData().GetLeaderboard(name);

			leaderboard.displayName = galaxy::api::Stats()->GetLeaderboardDisplayName(name);
			leaderboard.sortMethod = galaxy::api::Stats()->GetLeaderboardSortMethod(name);
			leaderboard.displayType = galaxy::api::Stats()->GetLeaderboardDisplayType(name);

			Leaderboard::Entries entires;
			for (uint32_t i = 0; i < entryCount; ++i)
			{
				Leaderboard::Entry entry;
				galaxy::api::Stats()->GetRequestedLeaderboardEntry(i, entry.rank, entry.score, entry.userID);
				entires.push_back(entry);
			}

			leaderboard.entries.swap(entires);
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Error occurred during OnLeaderboardEntriesRetrieveSuccess execution, error=%s", error.GetMsg());
		}
	}

	void LeaderboardEntriesRetrieveListener::OnLeaderboardEntriesRetrieveFailure(const char* name, galaxy::api::ILeaderboardEntriesRetrieveListener::FailureReason failureReason)
	{
		fprintf(stdout, "Failed to retrieve leaderboard entries, leaderboard name %s, reason %u\n", name, failureReason);
	}

	LeaderboardScoreUpdateListener::LeaderboardScoreUpdateListener(GameContext& _gameContext)
		: GameContextListener(_gameContext)
	{
	}

	void LeaderboardScoreUpdateListener::OnLeaderboardScoreUpdateSuccess(const char* name, int32_t score, uint32_t oldRank, uint32_t newRank)
	{
		fprintf(stdout, "Updated leaderboard entry, leaderboard name %s, score %d, old rank %u, new rank %u\n", name, score, oldRank, newRank);
		Leaderboard& leaderboard = gameContext.GetGameplayData().GetLeaderboard(name);

		Leaderboard::Entries::iterator it(leaderboard.entries.begin()), itEnd(leaderboard.entries.end());
		for (; it < itEnd; ++it)
		{
			if (it->userID == gameContext.GetUserID())
			{
				it->score = score;
				it->rank = newRank;
				return;
			}
		}

		Leaderboard::Entry entry;
		entry.rank = newRank;
		entry.score = score;
		entry.userID = gameContext.GetUserID();
		leaderboard.entries.push_back(entry);
	}

	void LeaderboardScoreUpdateListener::OnLeaderboardScoreUpdateFailure(const char* name, int32_t score, galaxy::api::ILeaderboardScoreUpdateListener::FailureReason failureReason)
	{
		fprintf(stdout, "Failed to update leaderboard entry, leaderboard name %s, score %d, reason %u\n", name, score, failureReason);
	}

	ListenerManager::~ListenerManager()
	{
		DeleteListeners();
	}

	void ListenerManager::CreateListeners(GameContext& gameContext)
	{
		listeners.push_back(new AuthListener(gameContext));
		listeners.push_back(new OperationalStateChangeListener());
		listeners.push_back(new PersonaDataChangedListener());
		listeners.push_back(new LobbyEnteredListener(gameContext));
		listeners.push_back(new LobbyCreatedListener(gameContext));
		listeners.push_back(new LobbyListListener(gameContext));
		listeners.push_back(new LobbyMemberStateListener());
		listeners.push_back(new LobbyMessageListener());
		listeners.push_back(new LobbyDataListener());
		listeners.push_back(new LobbyOwnerChangeListener());
		listeners.push_back(new NetworkingListener(gameContext));
		listeners.push_back(new ServerNetworkingListener(gameContext));
		listeners.push_back(new UserStatsAndAchievementsRetrieveListener(gameContext));
		listeners.push_back(new StatsAndAchievementsStoreListener(gameContext));
		listeners.push_back(new AchievementChangeListener(gameContext));
		listeners.push_back(new LeaderboardEntriesRetrieveListener(gameContext));
		listeners.push_back(new LeaderboardScoreUpdateListener(gameContext));
		listeners.push_back(new LeaderboardsRetrieveListener(gameContext));
	}

	void ListenerManager::DeleteListeners()
	{
		for (std::vector<galaxy::api::IGalaxyListener*>::iterator it = listeners.begin(); it != listeners.end(); )
		{
			delete *it;
			it = listeners.erase(it);
		}
	}

}
