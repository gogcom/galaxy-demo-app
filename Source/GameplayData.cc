#include "GameplayData.h"

namespace game
{

	GameplayData::GameplayData()
		: wereStatsAndAchievementsRequested(false)
		, wereLeaderboardsRequested(false)
	{
	}

	Achievements& GameplayData::GetUserAchievements(const galaxy::api::GalaxyID& userID)
	{
		UsersAchievements::iterator userAchievements = usersAchievements.find(userID);
		assert(userAchievements != usersAchievements.end());
		return userAchievements->second;
	}

	Statistics& GameplayData::GetUserStatistics(const galaxy::api::GalaxyID& userID)
	{
		UsersStatistics::iterator userStatistics = usersStatistics.find(userID);
		assert(userStatistics != usersStatistics.end());
		return userStatistics->second;
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

	bool GameplayData::GetStatsAndAchievementsStatus() const
	{
		return wereStatsAndAchievementsRequested;
	}

	void GameplayData::SetStatsAndAchievementStatus(bool _wereStatsAndAchievementsRequested)
	{
		wereStatsAndAchievementsRequested = _wereStatsAndAchievementsRequested;
	}

	bool GameplayData::GetLeaderboardsStatus() const
	{
		return wereLeaderboardsRequested;
	}

	void GameplayData::SetLeaderboardsStatus(bool _wereLeaderboardsRequested)
	{
		wereLeaderboardsRequested = _wereLeaderboardsRequested;
	}

}