#include "Achievements.h"

namespace game
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
		Achievement achievement;
		achievement.unlocked = false;
		achievement.unlockedTime = 0;
		achievement.displayName = "achievement_name";
		achievement.description = "achievement_description";
		achievement.visible = true;
		achievement.name = "achievement";
		achievements[achievement.name] = achievement;
		return achievements;
	}

}