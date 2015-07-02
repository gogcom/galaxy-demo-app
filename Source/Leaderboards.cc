#include "Leaderboards.h"

namespace game
{

	Leaderboard::Entry::Entry()
		: score(0)
		, rank(0)
	{
	}

	Leaderboard::Entry::Entry(const galaxy::api::GalaxyID& _userID, int32_t _score, uint32_t _rank)
		: userID(_userID)
		, score(_score)
		, rank(_rank)
	{
	}

	Leaderboards LeaderboardFactory::CreateDefaultLeaderboards()
	{
		Leaderboards leaderboards;
		Leaderboard numberLeaderboard;
		numberLeaderboard.name = "number_leaderboard";
		numberLeaderboard.displayName = "number leaderboard";
		numberLeaderboard.sortMethod = galaxy::api::LEADERBOARD_SORT_METHOD_DESCENDING;
		numberLeaderboard.displayType = galaxy::api::LEADERBOARD_DISPLAY_TYPE_NUMBER;
		leaderboards[numberLeaderboard.name] = numberLeaderboard;

		Leaderboard timeSecondsLeaderboard;
		timeSecondsLeaderboard.name = "time_seconds_leaderboard";
		timeSecondsLeaderboard.displayName = "time seconds leaderboard";
		timeSecondsLeaderboard.sortMethod = galaxy::api::LEADERBOARD_SORT_METHOD_ASCENDING;
		timeSecondsLeaderboard.displayType = galaxy::api::LEADERBOARD_DISPLAY_TYPE_TIME_SECONDS;
		leaderboards[timeSecondsLeaderboard.name] = timeSecondsLeaderboard;

		Leaderboard timeMilisecondsLeaderboard;
		timeMilisecondsLeaderboard.name = "time_miliseconds_leaderboard";
		timeMilisecondsLeaderboard.displayName = "time miliseconds leaderboard";
		timeMilisecondsLeaderboard.sortMethod = galaxy::api::LEADERBOARD_SORT_METHOD_DESCENDING;
		timeMilisecondsLeaderboard.displayType = galaxy::api::LEADERBOARD_DISPLAY_TYPE_TIME_MILLISECONDS;
		leaderboards[timeMilisecondsLeaderboard.name] = timeMilisecondsLeaderboard;
		return leaderboards;
	}
}