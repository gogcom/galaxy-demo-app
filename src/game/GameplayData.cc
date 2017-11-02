#include "GameplayData.h"

namespace galaxy
{
	namespace api
	{
		extern bool IsFullyInitialized;
	}
}

namespace gogtron
{

	GameplayData::GameplayData()
	{
	}

	Achievements& GameplayData::GetUserAchievements(const galaxy::api::GalaxyID& userID)
	{
		assert(userID.IsValid());

		auto& userAchievements = usersAchievements[userID];
		if (userAchievements.empty())
			userAchievements = AchievementsFactory::CreateDefaultAchievements();

		return userAchievements;
	}

	Statistics& GameplayData::GetUserStatistics(const galaxy::api::GalaxyID& userID)
	{
		assert(userID.IsValid());
		auto & userStatistics = usersStatistics[userID];
		if (userStatistics.empty())
			userStatistics = StatisticsFactory::CreateDefaultStatistics();

		return userStatistics;
	}

	void GameplayData::SetUserAchievements(const galaxy::api::GalaxyID& userID, const Achievements& achievements)
	{
		assert(userID.IsValid());
		usersAchievements[userID] = achievements;
	}

	void GameplayData::SetUserStatistics(const galaxy::api::GalaxyID& userID, const Statistics& statistics)
	{
		assert(userID.IsValid());
		usersStatistics[userID] = statistics;
	}

	Leaderboard& GameplayData::GetLeaderboard(const std::string& leaderboardName)
	{
		Leaderboards::iterator leaderboard = leaderboards.find(leaderboardName);
		assert(leaderboard != leaderboards.end());
		return leaderboard->second;
	}

	const Leaderboards& GameplayData::GetLeaderboards() const
	{
		return leaderboards;
	}

	void GameplayData::SetLeaderboards(const Leaderboards& _leaderboards)
	{
		leaderboards = _leaderboards;
	}

	void GameplayData::Init()
	{
		assert(galaxy::api::IsFullyInitialized && galaxy::api::User()->SignedIn());

		InitListeners();

		try
		{
			galaxy::api::Stats()->RequestUserStatsAndAchievements();
			galaxy::api::Stats()->RequestLeaderboards();
		}
		catch (const galaxy::api::IError& error)
		{
			galaxy::api::Logger()->Error("Failed to retrieve user stats and achievements: %s", error.GetMsg());
		}
	}

	void GameplayData::InitListeners()
	{
		assert(listeners.empty());

		listeners.emplace_back(std::make_unique<UserStatsAndAchievementsRetrieveListener>(*this));
		listeners.emplace_back(std::make_unique<AchievementChangeListener>(*this));
		listeners.emplace_back(std::make_unique<StatsAndAchievementsStoreListener>(*this));
		listeners.emplace_back(std::make_unique<LeaderboardsRetrieveListener>(*this));
		listeners.emplace_back(std::make_unique<LeaderboardEntriesRetrieveListener>(*this));
		listeners.emplace_back(std::make_unique<LeaderboardScoreUpdateListener>(*this));
	}

	GameplayData::UserStatsAndAchievementsRetrieveListener::UserStatsAndAchievementsRetrieveListener(GameplayData& _gameplayData)
		: gameplayData(_gameplayData)
	{
	}

	void GameplayData::UserStatsAndAchievementsRetrieveListener::OnUserStatsAndAchievementsRetrieveSuccess(galaxy::api::GalaxyID userID)
	{
		try
		{
			Achievements& userAchievements = gameplayData.GetUserAchievements(userID);
			Achievements::iterator userAchievementsIter(userAchievements.begin()), userAchievementsIterEnd(userAchievements.end());
			for (; userAchievementsIter != userAchievementsIterEnd; ++userAchievementsIter)
			{
				galaxy::api::Stats()->GetAchievement(userAchievementsIter->second.name.c_str(), userAchievementsIter->second.unlocked, userAchievementsIter->second.unlockedTime, userID);
				userAchievementsIter->second.description = galaxy::api::Stats()->GetAchievementDescription(userAchievementsIter->second.name.c_str());
				userAchievementsIter->second.displayName = galaxy::api::Stats()->GetAchievementDisplayName(userAchievementsIter->second.name.c_str());
				userAchievementsIter->second.visible = galaxy::api::Stats()->IsAchievementVisible(userAchievementsIter->second.name.c_str());
			}

			Statistics& usersStatistics = gameplayData.GetUserStatistics(userID);
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
		}
		catch (const galaxy::api::IError& error)
		{
			galaxy::api::Logger()->Error("Error occurred during OnUserStatsAndAchievementsRetrieveSuccess execution, error=%s\n", error.GetMsg());
		}
	}

	void GameplayData::UserStatsAndAchievementsRetrieveListener::OnUserStatsAndAchievementsRetrieveFailure(galaxy::api::GalaxyID userID, FailureReason failureReason)
	{
	}

	GameplayData::AchievementChangeListener::AchievementChangeListener(GameplayData& _gameplayData)
		: gameplayData(_gameplayData)
	{
	}

	void GameplayData::AchievementChangeListener::OnAchievementUnlocked(const char* name)
	{
		try
		{
			auto&& userID = galaxy::api::User()->GetGalaxyID();
			Achievements& userAchievements = gameplayData.GetUserAchievements(userID);
			Achievements::iterator userAchievement = userAchievements.find(name);
			galaxy::api::Stats()->GetAchievement(name, userAchievement->second.unlocked, userAchievement->second.unlockedTime, userID);
		}
		catch (const galaxy::api::IError& error)
		{
			galaxy::api::Logger()->Error("Error occurred during OnAchievementUnlocked execution, error=%s\n", error.GetMsg());
		}
	}

	GameplayData::StatsAndAchievementsStoreListener::StatsAndAchievementsStoreListener(GameplayData& _gameplayData)
		: gameplayData(_gameplayData)
	{
	}

	void GameplayData::StatsAndAchievementsStoreListener::OnUserStatsAndAchievementsStoreSuccess()
	{
		try
		{
			auto&& userID = galaxy::api::User()->GetGalaxyID();
			Statistics& userStatistics = gameplayData.GetUserStatistics(userID);
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

			Achievements& userAchievements = gameplayData.GetUserAchievements(userID);
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
			galaxy::api::Logger()->Error("Error occurred during OnUserStatsAndAchievementsStoreSuccess execution, error=%s\n", error.GetMsg());
		}
	}

	void GameplayData::StatsAndAchievementsStoreListener::OnUserStatsAndAchievementsStoreFailure(FailureReason failureReason)
	{
	}

	GameplayData::LeaderboardsRetrieveListener::LeaderboardsRetrieveListener(GameplayData& /*_gameplayData*/)
	{
	}

	void GameplayData::LeaderboardsRetrieveListener::OnLeaderboardsRetrieveSuccess()
	{
	}

	void GameplayData::LeaderboardsRetrieveListener::OnLeaderboardsRetrieveFailure(FailureReason failureReason)
	{
	}

	GameplayData::LeaderboardEntriesRetrieveListener::LeaderboardEntriesRetrieveListener(GameplayData& _gameplayData)
		: gameplayData(_gameplayData)
	{
	}

	void GameplayData::LeaderboardEntriesRetrieveListener::OnLeaderboardEntriesRetrieveSuccess(const char* name, uint32_t entryCount)
	{
		try
		{
			Leaderboard& leaderboard = gameplayData.GetLeaderboard(name);

			leaderboard.displayName = galaxy::api::Stats()->GetLeaderboardDisplayName(name);
			leaderboard.sortMethod = galaxy::api::Stats()->GetLeaderboardSortMethod(name);
			leaderboard.displayType = galaxy::api::Stats()->GetLeaderboardDisplayType(name);

			Leaderboard::Entries entires;
			for (uint32_t i = 0; i < entryCount; ++i)
			{
				Leaderboard::Entry entry;
				galaxy::api::Stats()->GetRequestedLeaderboardEntry(i, entry.rank, entry.score, entry.userID);
				const char* userName = galaxy::api::Friends()->GetFriendPersonaName(entry.userID);
				galaxy::api::Logger()->Info("[%u] user name=%s, userID=%llu, rank=%u, score=%d\n", i, userName, entry.userID.ToUint64(), entry.rank, entry.score);
				entires.push_back(entry);
			}

			leaderboard.entries.swap(entires);
		}
		catch (const galaxy::api::IError& error)
		{
			galaxy::api::Logger()->Error("Error occurred during OnLeaderboardEntriesRetrieveSuccess execution, error=%s", error.GetMsg());
		}
	}

	void GameplayData::LeaderboardEntriesRetrieveListener::OnLeaderboardEntriesRetrieveFailure(const char* name, FailureReason failureReason)
	{
	}

	GameplayData::LeaderboardScoreUpdateListener::LeaderboardScoreUpdateListener(GameplayData& _gameplayData)
		: gameplayData(_gameplayData)
	{
	}

	void GameplayData::LeaderboardScoreUpdateListener::OnLeaderboardScoreUpdateSuccess(const char* name, int32_t score, uint32_t oldRank, uint32_t newRank)
	{
		auto&& userID = galaxy::api::User()->GetGalaxyID();
		Leaderboard& leaderboard = gameplayData.GetLeaderboard(name);
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

	void GameplayData::LeaderboardScoreUpdateListener::OnLeaderboardScoreUpdateFailure(const char* name, int32_t score, FailureReason failureReason)
	{
	}

}
