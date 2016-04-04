#ifndef GOGTRON_GAMEPLAY_DATA_H
#define GOGTRON_GAMEPLAY_DATA_H

#include "Achievements.h"
#include "Statistics.h"
#include "Leaderboards.h"

namespace gogtron
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

        bool GetStatsAndAchievementsStatus() const;

        void SetStatsAndAchievementStatus(bool wereStatsAndAchievementsRequested);

        bool GetLeaderboardsStatus() const;

        void SetLeaderboardsStatus(bool wereLeaderboardsRequested);

    private:

        bool wereStatsAndAchievementsRequested;
        bool wereLeaderboardsRequested;
        UsersAchievements usersAchievements;
        UsersStatistics usersStatistics;
        Leaderboards leaderboards;
    };

}

#endif