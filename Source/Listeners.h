#ifndef GAME_LISTENERS_H
#define GAME_LISTENERS_H

#include "GameContext.h"
#include <cstdio>
#include <vector>
#include <galaxy/GalaxyApi.h>

namespace game
{

	template <typename T>
	class GameContextListener : public T
	{
	public:

		GameContextListener(GameContext& _gameContext)
			: gameContext(_gameContext)
		{
		}

	protected:

		GameContext& gameContext;
	};

	// called after entering a lobby (after joining or creating it)
	class LobbyEnteredListener : public GameContextListener<galaxy::api::GlobalLobbyEnteredListener>
	{
	public:

		LobbyEnteredListener(GameContext& gameContext);

		virtual void OnLobbyEntered(const galaxy::api::GalaxyID& lobbyID, galaxy::api::LobbyEnterResult result);
	};

	// called when another member joins of leaves a lobby
	class LobbyMemberStateListener : public galaxy::api::GlobalLobbyMemberStateListener
	{
	public:

		virtual void OnLobbyMemberStateChanged(const galaxy::api::GalaxyID& lobbyID, const galaxy::api::GalaxyID& memberID, galaxy::api::LobbyMemberStateChange memberStateChange);
	};

	// called when creating a lobby
	class LobbyCreatedListener : public GameContextListener<galaxy::api::GlobalLobbyCreatedListener>
	{
	public:

		LobbyCreatedListener(GameContext& gameContext);

		virtual void OnLobbyCreated(const galaxy::api::GalaxyID& lobbyID, galaxy::api::LobbyCreateResult result);
	};

	// called when retrieving existing lobbies
	class LobbyListListener : public GameContextListener<galaxy::api::GlobalLobbyListListener>
	{
	public:

		LobbyListListener(GameContext& gameContext);

		virtual void OnLobbyList(uint32_t lobbyCount, bool ioFailure);
	};

	// called when a P2P packet comes to the client
	class NetworkingListener : public GameContextListener<galaxy::api::GlobalNetworkingListener>
	{
	public:

		NetworkingListener(GameContext& gameContext);

		virtual void OnP2PPacketAvailable(uint32_t msgSize, uint8_t channel);
	};

	// called when a P2P packet comes to the server (a packet that was addressed to owned lobby)
	class ServerNetworkingListener : public GameContextListener<galaxy::api::GlobalServerNetworkingListener>
	{
	public:

		ServerNetworkingListener(GameContext& gameContext);

		virtual void OnServerP2PPacketAvailable(uint32_t msgSize, uint8_t channel);

	private:

		bool IsCurrentUserLobbyOwner() const;
	};

	// called when signing in or loosing authentication
	class AuthListener : public GameContextListener<galaxy::api::GlobalAuthListener>
	{
	public:

		AuthListener(GameContext& gameContext);

		virtual void OnAuthSuccess();

		virtual void OnAuthFailure(FailureReason reason);

		virtual void OnAuthLost();
	};

	// called when operational state changes
	class OperationalStateChangeListener : public galaxy::api::GlobalOperationalStateChangeListener
	{
	public:

		virtual void OnOperationalStateChanged(uint32_t operationalState);
	};

	// called when persona data changes (e.g. when joining lobby)
	class PersonaDataChangedListener : public galaxy::api::GlobalPersonaDataChangedListener
	{
	public:

		virtual void OnPersonaDataChanged(galaxy::api::GalaxyID userID, uint32_t personaStateChange);
	};

	// called when anyone in the lobby sends a message (also for the sender)
	class LobbyMessageListener : public galaxy::api::GlobalLobbyMessageListener
	{
	public:

		virtual void OnLobbyMessageReceived(const galaxy::api::GalaxyID& lobbyID, const galaxy::api::GalaxyID& senderID, uint32_t messageID, uint32_t messageLength);
	};

	// called when a lobby member changes his data, or when the lobby owner changes the lobby data
	class LobbyDataListener : public galaxy::api::GlobalLobbyDataListener
	{
	public:

		virtual void OnLobbyDataUpdated(const galaxy::api::GalaxyID& lobbyID, const galaxy::api::GalaxyID& memberID);
	};

	// called when the lobby owner changes (typically the old lobby owner leaves)
	class LobbyOwnerChangeListener : public galaxy::api::GlobalLobbyOwnerChangeListener
	{
	public:

		virtual void OnLobbyOwnerChanged(const galaxy::api::GalaxyID& lobbyID, const galaxy::api::GalaxyID& newOwnerID);
	};


	class UserStatsAndAchievementsRetrieveListener : public GameContextListener<galaxy::api::GlobalUserStatsAndAchievementsRetrieveListener>
	{
	public:

		UserStatsAndAchievementsRetrieveListener(GameContext& gameContext);

		virtual void OnUserStatsAndAchievementsRetrieveSuccess(galaxy::api::GalaxyID userID);

		virtual void OnUserStatsAndAchievementsRetrieveFailure(galaxy::api::GalaxyID userID, galaxy::api::IUserStatsAndAchievementsRetrieveListener::FailureReason failureReason);
	};

	class StatsAndAchievementsStoreListener : public GameContextListener<galaxy::api::GlobalStatsAndAchievementsStoreListener>
	{
	public:

		StatsAndAchievementsStoreListener(GameContext& gameContext);

		virtual void OnUserStatsAndAchievementsStoreSuccess();

		virtual void OnUserStatsAndAchievementsStoreFailure(galaxy::api::IStatsAndAchievementsStoreListener::FailureReason failureReason);
	};

	class AchievementChangeListener : public GameContextListener<galaxy::api::GlobalAchievementChangeListener>
	{
	public:

		AchievementChangeListener(GameContext& gameContext);

		virtual void OnAchievementUnlocked(const char* name);
	};

	class LeaderboardsRetrieveListener : public GameContextListener<galaxy::api::GlobalLeaderboardsRetrieveListener>
	{
	public:

		LeaderboardsRetrieveListener(GameContext& gameContext);

		virtual void OnLeaderboardsRetrieveSuccess();

		virtual void OnLeaderboardsRetrieveFailure(galaxy::api::ILeaderboardsRetrieveListener::FailureReason failureReason);
	};

	class LeaderboardEntriesRetrieveListener : public GameContextListener<galaxy::api::GlobalLeaderboardEntriesRetrieveListener>
	{
	public:

		LeaderboardEntriesRetrieveListener(GameContext& gameContext);

		virtual void OnLeaderboardEntriesRetrieveSuccess(const char* name, uint32_t entryCount);

		virtual void OnLeaderboardEntriesRetrieveFailure(const char* name, galaxy::api::ILeaderboardEntriesRetrieveListener::FailureReason failureReason);
	};

	class LeaderboardScoreUpdateListener : public GameContextListener<galaxy::api::GlobalLeaderboardScoreUpdateListener>
	{
	public:

		LeaderboardScoreUpdateListener(GameContext& gameContext);

		virtual void OnLeaderboardScoreUpdateSuccess(const char* name, int32_t score, uint32_t oldRank, uint32_t newRank);

		virtual void OnLeaderboardScoreUpdateFailure(const char* name, int32_t score, galaxy::api::ILeaderboardScoreUpdateListener::FailureReason failureReason);
	};

	class ListenerManager
	{
	public:

		~ListenerManager();

		void CreateListeners(GameContext& gameContext);

		void DeleteListeners();

	private:

		std::vector<galaxy::api::IGalaxyListener*> listeners;
	};

}

#endif /* LISTENERS_H */