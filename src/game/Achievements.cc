#include "Achievements.h"

namespace galaxy::demo
{

	Achievement::Achievement(const std::string& _name)
		: name(_name)
		, unlocked(false)
		, unlockedTime(0)
		, displayName("")
		, description("")
		, visible(false)
	{
	}

	Achievements AchievementsFactory::CreateDefaultAchievements()
	{
		Achievements achievements;

		{
			Achievement achievement;
			achievement.unlocked = false;
			achievement.unlockedTime = 0;
			achievement.displayName = "";
			achievement.description = "";
			achievement.visible = true;
			achievement.name = "first_win";
			achievements[achievement.name] = achievement;
		}

		{
			Achievement achievement;
			achievement.unlocked = false;
			achievement.unlockedTime = 0;
			achievement.displayName = "";
			achievement.description = "";
			achievement.visible = true;
			achievement.name = "quick_win";
			achievements[achievement.name] = achievement;
		}

		return achievements;
	}

}