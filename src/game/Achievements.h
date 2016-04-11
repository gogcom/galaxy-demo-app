#ifndef GOGTRON_ACHIEVEMENTS_H
#define GOGTRON_ACHIEVEMENTS_H

#include <string>
#include <map>
#include <stdint.h>
#include <galaxy/GalaxyID.h>

namespace gogtron
{

    struct Achievement
    {
        std::string name;
        bool unlocked;
        uint32_t unlockedTime;
        std::string displayName;
        std::string description;
        bool visible;

        Achievement(const std::string& _name = "");
    };

    typedef std::map<std::string, Achievement> Achievements;

    typedef std::map<galaxy::api::GalaxyID, Achievements> UsersAchievements;

    class AchievementsFactory
    {
    public:

        static Achievements CreateDefaultAchievements();
    };

}

#endif