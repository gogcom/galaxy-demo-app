#include "Leaderboards.h"

using namespace gogtron;

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

	{
		Leaderboard leaderboard;
		leaderboard.name = "best_winners";
		leaderboard.displayName = "";
		leaderboard.sortMethod = galaxy::api::LEADERBOARD_SORT_METHOD_DESCENDING;
		leaderboard.displayType = galaxy::api::LEADERBOARD_DISPLAY_TYPE_NUMBER;
		leaderboards[leaderboard.name] = leaderboard;
	}

	{
		Leaderboard leaderboard;
		leaderboard.name = "quickest_winners";
		leaderboard.displayName = "";
		leaderboard.sortMethod = galaxy::api::LEADERBOARD_SORT_METHOD_DESCENDING;
		leaderboard.displayType = galaxy::api::LEADERBOARD_DISPLAY_TYPE_NUMBER;
		leaderboards[leaderboard.name] = leaderboard;
	}

	return leaderboards;
}