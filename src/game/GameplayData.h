#ifndef GALAXY_DEMO_GAMEPLAY_DATA_H
#define GALAXY_DEMO_GAMEPLAY_DATA_H

#include "Achievements.h"
#include "Statistics.h"
#include "Leaderboards.h"
#include <galaxy/GalaxyApi.h>
#include <memory>

namespace galaxy::demo
{

	class User;

	class GameplayData
	{
	public:

		GameplayData();

		Achievements& GetUserAchievements(const galaxy::api::GalaxyID& userID);

		Statistics& GetUserStatistics(const galaxy::api::GalaxyID& userID);

		void SetUserAchievements(const galaxy::api::GalaxyID& userID, const Achievements& achievements);

		void SetUserStatistics(const galaxy::api::GalaxyID& userID, const Statistics& statistics);

		Leaderboard& GetLeaderboard(const std::string& leaderboardName);

		const Leaderboards& GetLeaderboards() const;

		void SetLeaderboards(const Leaderboards& leaderboards);

		void Init();

		void RefreshLeaderboardsEntries() const;

	private:

		void InitListeners();

		class UserStatsAndAchievementsRetrieveListener : public galaxy::api::GlobalUserStatsAndAchievementsRetrieveListener
		{
		public:

			UserStatsAndAchievementsRetrieveListener(GameplayData& gameplayData);

			virtual void OnUserStatsAndAchievementsRetrieveSuccess(galaxy::api::GalaxyID userID) override;

			virtual void OnUserStatsAndAchievementsRetrieveFailure(galaxy::api::GalaxyID userID, FailureReason failureReason) override;

		private:

			GameplayData& gameplayData;
		};

		class AchievementChangeListener : public galaxy::api::GlobalAchievementChangeListener
		{
		public:

			AchievementChangeListener(GameplayData& gameplayData);

			virtual void OnAchievementUnlocked(const char* name) override;

		private:

			GameplayData& gameplayData;
		};

		class StatsAndAchievementsStoreListener : public galaxy::api::GlobalStatsAndAchievementsStoreListener
		{
		public:

			StatsAndAchievementsStoreListener(GameplayData& gameplayData);

			virtual void OnUserStatsAndAchievementsStoreSuccess() override;

			virtual void OnUserStatsAndAchievementsStoreFailure(FailureReason failureReason) override;

		private:

			GameplayData& gameplayData;
		};

		class LeaderboardsRetrieveListener : public galaxy::api::GlobalLeaderboardsRetrieveListener
		{
		public:

			LeaderboardsRetrieveListener(GameplayData& gameplayData);

			virtual void OnLeaderboardsRetrieveSuccess() override;

			virtual void OnLeaderboardsRetrieveFailure(FailureReason failureReason) override;

			GameplayData& gameplayData;
		};

		class LeaderboardEntriesRetrieveListener : public galaxy::api::GlobalLeaderboardEntriesRetrieveListener
		{
		public:

			LeaderboardEntriesRetrieveListener(GameplayData& gameplayData);

			virtual void OnLeaderboardEntriesRetrieveSuccess(const char* name, uint32_t entryCount) override;

			virtual void OnLeaderboardEntriesRetrieveFailure(const char* name, FailureReason failureReason) override;

		private:

			GameplayData& gameplayData;
		};

		class LeaderboardScoreUpdateListener : public galaxy::api::GlobalLeaderboardScoreUpdateListener
		{
		public:

			LeaderboardScoreUpdateListener(GameplayData& gameplayData);

			virtual void OnLeaderboardScoreUpdateSuccess(const char* name, int32_t score, uint32_t oldRank, uint32_t newRank) override;

			virtual void OnLeaderboardScoreUpdateFailure(const char* name, int32_t score, FailureReason failureReason) override;

		private:

			GameplayData& gameplayData;
		};

		std::vector<std::unique_ptr<galaxy::api::IGalaxyListener>> listeners;

		UsersAchievements usersAchievements;
		UsersStatistics usersStatistics;
		Leaderboards leaderboards;
	};

}

#endif