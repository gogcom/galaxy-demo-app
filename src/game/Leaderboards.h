#ifndef GALAXY_DEMO_LEADERBOARDS_H
#define GALAXY_DEMO_LEADERBOARDS_H

#include <galaxy/IStats.h>
#include <vector>
#include <map>
#include <string>

namespace galaxy::demo
{

	struct Leaderboard
	{
		std::string name;
		std::string displayName;
		galaxy::api::LeaderboardSortMethod sortMethod;
		galaxy::api::LeaderboardDisplayType displayType;
		struct Entry;
		typedef std::vector<Entry> Entries;
		Entries entries;

		struct Entry
		{
			galaxy::api::GalaxyID userID;
			int32_t score;
			uint32_t rank;

			Entry();

			Entry(const galaxy::api::GalaxyID& _userID, int32_t _score, uint32_t _rank = 0);
		};
	};

	typedef std::map<std::string, Leaderboard> Leaderboards;

	class LeaderboardFactory
	{
	public:

		static Leaderboards CreateDefaultLeaderboards();
	};

}

#endif