#include "Input.h"
#include "Kbhit.h"
#include "GameContext.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <ctime>
#include <galaxy/GalaxyApi.h>

namespace game
{

	void HelpCommand::Execute(GameContext& gameContext)
	{
		gameContext.PrintGameControls();
	}

	void QuitCommand::Execute(GameContext& gameContext)
	{
		gameContext.SetGameState(GameContext::QUIT);
	}

	void SendLobbyMessageCommand::Execute(GameContext& gameContext)
	{
		try
		{
			const char* message = "lobby message (broadcast to all members)";
			fprintf(stdout, "Sending lobby message with content '%s'\n", message);
			galaxy::api::Matchmaking()->SendLobbyMessage(gameContext.GetLobbyID(), message, strlen(message));
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Failed to call SendLobbyMessage, error=%s\n", error.GetMsg());
		}
	}

	void SendPacketToLobbyUserCommand::Execute(GameContext& gameContext)
	{
		try
		{
			const char* packet_content = "packet to lobby user";
			fprintf(stdout, "Sending packet to lobby user with content: '%s'\n", packet_content);
			galaxy::api::Networking()->SendP2PPacket(gameContext.GetUserID(), packet_content, strlen(packet_content), galaxy::api::P2P_SEND_RELIABLE, 0);
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Failed to call SendP2PPacket, error=%s\n", error.GetMsg());
		}
	}

	void SendPacketToLobbyOwnerCommand::Execute(GameContext& gameContext)
	{
		try
		{
			const char* packet_content = "packet to lobby owner";
			fprintf(stdout, "Sending packet to lobby owner with content '%s'\n", packet_content);
			galaxy::api::Networking()->SendP2PPacket(gameContext.GetLobbyID(), packet_content, strlen(packet_content), galaxy::api::P2P_SEND_RELIABLE, 0);
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Failed to call SendP2PPacket, error=%s\n", error.GetMsg());
		}
	}

	void SetLobbyMemberDataCommand::Execute(GameContext& gameContext)
	{
		try
		{
			const char* key = "lobby member data key";
			const char* value = "lobby member data value";
			fprintf(stdout, "Setting lobby member data '%s' - > '%s'\n", key, value);
			galaxy::api::Matchmaking()->SetLobbyMemberData(gameContext.GetLobbyID(), key, value);
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Failed to call SetLobbyMemberData, error=%s\n", error.GetMsg());
		}
	}

	void SetLobbyDataCommand::Execute(GameContext& gameContext)
	{
		try
		{
			const char* key = "lobby data key";
			const char* value = "lobby data value";
			fprintf(stdout, "Setting lobby data '%s' - > '%s'\n", key, value);
			galaxy::api::Matchmaking()->SetLobbyData(gameContext.GetLobbyID(), key, value);
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Failed to call SetLobbyData, error=%s\n", error.GetMsg());
		}
	}

	void RequestUserStatsAndAchievementsCommand::Execute(GameContext& gameContext)
	{
		try
		{
			fprintf(stdout, "Requesting own achievements and statistics...\n");
			galaxy::api::Stats()->RequestUserStatsAndAchievements();
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Failed to call RequestUserStatsAndAchievements, error=%s\n", error.GetMsg());
		}
	}

	void UnlockAchievementCommand::Execute(GameContext& gameContext)
	{
		if (!gameContext.GetGameplayData().GetStatsAndAchievementsStatus())
		{
			fprintf(stdout, "Unable to call UnlockAchievement; call RequestUserStatsAndAchievements first\n");
			return;
		}

		Achievements& userAchievements = gameContext.GetGameplayData().GetUserAchievements(gameContext.GetUserID());
		Achievements::iterator iter(userAchievements.begin()), iterEnd(userAchievements.end());
		for (; iter != iterEnd; ++iter)
		{
			if (!iter->second.unlocked)
			{
				try
				{
					fprintf(stdout, "Unlocking achievement '%s'\n", iter->second.name.c_str());
					galaxy::api::Stats()->SetAchievement(iter->second.name.c_str());
					iter->second.unlocked = true;
					return;
				}
				catch (const galaxy::api::IError& error)
				{
					fprintf(stderr, "Failed to call SetAchievement, error=%s\n", error.GetMsg());
				}
			}
		}

		fprintf(stdout, "All achievements are already unlocked\n");
	}

	void ClearAchievementCommand::Execute(GameContext& gameContext)
	{
		if (!gameContext.GetGameplayData().GetStatsAndAchievementsStatus())
		{
			fprintf(stdout, "Unable to call ClearAchievement; call RequestUserStatsAndAchievements first\n");
			return;
		}

		Achievements& userAchievements = gameContext.GetGameplayData().GetUserAchievements(gameContext.GetUserID());
		Achievements::iterator iter(userAchievements.begin()), iterEnd(userAchievements.end());
		for (; iter != iterEnd; ++iter)
		{
			if (iter->second.unlocked)
			{
				try
				{
					fprintf(stdout, "Clearing achievement '%s'\n", iter->second.name.c_str());
					galaxy::api::Stats()->ClearAchievement(iter->second.name.c_str());
					iter->second.unlocked = false;
					iter->second.unlockedTime = 0;
					return;
				}
				catch (const galaxy::api::IError& error)
				{
					fprintf(stderr, "Failed to call ClearAchievement, error=%s\n", error.GetMsg());
				}
			}
		}

		fprintf(stdout, "All achievements are locked\n");
	}

	void SetIntStatisticCommand::Execute(GameContext& gameContext)
	{

		if (!gameContext.GetGameplayData().GetStatsAndAchievementsStatus())
		{
			fprintf(stdout, "Unable to call SetIntStatistic; call RequestUserStatsAndAchievements first\n");
			return;
		}

		Statistics& userStatistics = gameContext.GetGameplayData().GetUserStatistics(gameContext.GetUserID());
		Statistics::iterator iter(userStatistics.begin()), iterEnd(userStatistics.end());
		for (; iter != iterEnd; ++iter)
		{
			if (iter->second.GetType() == Statistic::INT)
			{
				try
				{
					fprintf(stdout, "Setting int statistic '%s'\n", iter->second.GetName().c_str());
					iter->second.SetInt(iter->second.GetInt() + 10);
					galaxy::api::Stats()->SetStatInt(iter->second.GetName().c_str(), iter->second.GetInt());
					return;
				}
				catch (const galaxy::api::IError& error)
				{
					fprintf(stderr, "Failed to call SetStatInt, %s\n", error.GetMsg());
				}
			}
		}
	}

	void SetFloatStatisticCommand::Execute(GameContext& gameContext)
	{
		if (!gameContext.GetGameplayData().GetStatsAndAchievementsStatus())
		{
			fprintf(stdout, "Unable to call SetFloatStatistic; call RequestUserStatsAndAchievements first\n");
			return;
		}

		Statistics& userStatistics = gameContext.GetGameplayData().GetUserStatistics(gameContext.GetUserID());
		Statistics::iterator iter(userStatistics.begin()), iterEnd(userStatistics.end());
		for (; iter != iterEnd; ++iter)
		{
			if (iter->second.GetType() == Statistic::FLOAT)
			{
				try
				{
					fprintf(stdout, "Setting float statistic '%s'\n", iter->second.GetName().c_str());
					iter->second.SetFloat(iter->second.GetFloat() + 12.5f);
					galaxy::api::Stats()->SetStatFloat(iter->second.GetName().c_str(), iter->second.GetFloat());
					return;
				}
				catch (const galaxy::api::IError& error)
				{
					fprintf(stderr, "Failed to call SetStatFloat, error=%s\n", error.GetMsg());
				}
			}
		}
	}

	void SetAvgStatisticCommand::Execute(GameContext& gameContext)
	{
		if (!gameContext.GetGameplayData().GetStatsAndAchievementsStatus())
		{
			fprintf(stdout, "Unable to call SetAvgStatistic; call RequestUserStatsAndAchievements first\n");
			return;
		}

		Statistics& userStatistics = gameContext.GetGameplayData().GetUserStatistics(gameContext.GetUserID());
		Statistics::iterator iter(userStatistics.begin()), iterEnd(userStatistics.end());
		for (; iter != iterEnd; ++iter)
		{
			if (iter->second.GetType() == Statistic::AVG)
			{
				try
				{
					fprintf(stdout, "Setting avg statistic '%s'\n", iter->second.GetName().c_str());
					const float count = 100;
					const double session = 15;
					galaxy::api::Stats()->UpdateAvgRateStat(iter->second.GetName().c_str(), count, session);
					return;
				}
				catch (const galaxy::api::IError& error)
				{
					fprintf(stderr, "Failed to call UpdateAvgRateStat, error=%s\n", error.GetMsg());
				}
			}
		}
	}

	void StoreStatsAndAchievementsCommand::Execute(GameContext& gameContext)
	{
		if (!gameContext.GetGameplayData().GetStatsAndAchievementsStatus())
		{
			fprintf(stdout, "Unable to call StoreStatsAndAchievements; call RequestUserStatsAndAchievements first\n");
			return;
		}

		try
		{
			fprintf(stdout, "Storing statistics and achievements...\n");
			galaxy::api::Stats()->StoreStatsAndAchievements();
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Failed to call StoreStatsAndAchievements, error=%s\n", error.GetMsg());
		}
	}

	void RequestLeaderboardsCommand::Execute(GameContext& gameContext)
	{
		try
		{
			fprintf(stdout, "Requesting leaderboards...\n");
			galaxy::api::Stats()->RequestLeaderboards();
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Failed to call RequestLeaderboards, error=%s\n", error.GetMsg());
		}
	}

	void RequestLeaderboardEntriesGlobalCommnad::Execute(GameContext& gameContext)
	{
		if (!gameContext.GetGameplayData().GetLeaderboardsStatus())
		{
			fprintf(stdout, "Unable to call RequestLeaderboardEntriesGlobal; call RequestLeaderboards first\n");
			return;
		}

		const Leaderboards& leaderboards = gameContext.GetGameplayData().GetLeaderboards();
		Leaderboards::const_iterator it(leaderboards.begin()), itEnd(leaderboards.end());
		for (; it != itEnd; ++it)
		{
			try
			{
				fprintf(stdout, "Requesting leaderboard entries global '%s'...\n", it->first.c_str());
				galaxy::api::Stats()->RequestLeaderboardEntriesGlobal(it->first.c_str(), 0, 10);
			}
			catch (const galaxy::api::IError& error)
			{
				fprintf(stderr, "Failed to call RequestLeaderboardEntriesGlobal, error=%s\n", error.GetMsg());
			}
		}
	}

	void RequestLeaderboardEntriesAroundUserCommand::Execute(GameContext& gameContext)
	{
		if (!gameContext.GetGameplayData().GetLeaderboardsStatus())
		{
			fprintf(stdout, "Unable to call RequestLeaderboardEntriesAroundUser; call RequestLeaderboards first\n");
			return;
		}

		const Leaderboards& leaderboards = gameContext.GetGameplayData().GetLeaderboards();
		Leaderboards::const_iterator it(leaderboards.begin()), itEnd(leaderboards.end());
		for (; it != itEnd; ++it)
		{
			try
			{
				fprintf(stdout, "Requesting leaderboard entries global '%s'...\n", it->first.c_str());
				galaxy::api::Stats()->RequestLeaderboardEntriesAroundUser(it->first.c_str(), -2, 2);
			}
			catch (const galaxy::api::IError& error)
			{
				fprintf(stderr, "Failed to call RequestLeaderboardEntriesAroundUser, error=%s\n", error.GetMsg());
			}
		}
	}

	void RequestLeaderboardEntriesForUsersCommand::Execute(GameContext& gameContext)
	{
		if (!gameContext.GetGameplayData().GetLeaderboardsStatus())
		{
			fprintf(stdout, "Unable to call RequestLeaderboardEntriesForUsers; call RequestLeaderboards first\n");
			return;
		}

		galaxy::api::GalaxyID users[] = { gameContext.GetUserID() };
		const Leaderboards& leaderboards = gameContext.GetGameplayData().GetLeaderboards();
		Leaderboards::const_iterator it(leaderboards.begin()), itEnd(leaderboards.end());
		for (; it != itEnd; ++it)
		{
			try
			{
				fprintf(stdout, "Requesting leaderboard entries global '%s'...\n", it->first.c_str());
				galaxy::api::Stats()->RequestLeaderboardEntriesForUsers(it->first.c_str(), users, sizeof(users) / sizeof(galaxy::api::GalaxyID));
			}
			catch (const galaxy::api::IError& error)
			{
				fprintf(stderr, "Failed to call RequestLeaderboardEntriesForUsers, error=%s\n", error.GetMsg());
			}
		}
	}

	void SetLeaderboardScoreCommand::Execute(GameContext& gameContext)
	{
		if (!gameContext.GetGameplayData().GetLeaderboardsStatus())
		{
			fprintf(stdout, "Unable to call SetLeaderboardScore; call RequestLeaderboards first\n");
			return;
		}

		const Leaderboards& leaderboards = gameContext.GetGameplayData().GetLeaderboards();
		Leaderboards::const_iterator it(leaderboards.begin()), itEnd(leaderboards.end());
		for (; it != itEnd; ++it)
		{
			const Leaderboard::Entries& entries = it->second.entries;
			uint32_t lastScore = 0;
			Leaderboard::Entries::const_iterator entryIter(entries.begin()), entryIterEnd(entries.end());
			for (; entryIter != entryIterEnd; ++entryIter)
			{
				if (entryIter->userID == gameContext.GetUserID())
				{
					lastScore = entryIter->score;
					break;
				}
			}

			try
			{
				fprintf(stdout, "Setting leaderboard entry score '%u'...\n", lastScore + 10);
				galaxy::api::Stats()->SetLeaderboardScore(it->first.c_str(), lastScore + 10, true);
			}
			catch (const galaxy::api::IError& error)
			{
				fprintf(stderr, "Failed to call SetLeaderboardScore, error=%s\n", error.GetMsg());
			}
		}
	}

	InputHandler::InputHandler()
	{
		registry['h'] = new HelpCommand;
		registry['q'] = new QuitCommand;
		registry['s'] = new SendLobbyMessageCommand;
		registry['k'] = new SendPacketToLobbyUserCommand;
		registry['p'] = new SendPacketToLobbyOwnerCommand;
		registry['m'] = new SetLobbyMemberDataCommand;
		registry['n'] = new SetLobbyDataCommand;
		registry['r'] = new RequestUserStatsAndAchievementsCommand;
		registry['u'] = new UnlockAchievementCommand;
		registry['c'] = new ClearAchievementCommand;
		registry['t'] = new StoreStatsAndAchievementsCommand;
		registry['i'] = new SetIntStatisticCommand;
		registry['f'] = new SetFloatStatisticCommand;
		registry['a'] = new SetAvgStatisticCommand;
		registry['l'] = new RequestLeaderboardsCommand;
		registry['g'] = new RequestLeaderboardEntriesGlobalCommnad;
		registry['d'] = new RequestLeaderboardEntriesAroundUserCommand;
		registry['o'] = new RequestLeaderboardEntriesForUsersCommand;
		registry['e'] = new SetLeaderboardScoreCommand;
	}

	void InputHandler::ReadCommands(std::list<ICommand*>& commands)
	{
		if (_kbhit())
		{
			std::map<const char, ICommand*>::const_iterator commandFound = registry.find(_getch());
			if (commandFound != registry.end())
				commands.push_back(commandFound->second);
		}
	}

	char InputHandler::ReadKey() const
	{
		return _getch();
	}

	InputHandler::~InputHandler()
	{
		for (std::map<const char, ICommand*>::iterator it = registry.begin(); it != registry.end(); ++it)
		{
			delete it->second;
		}
	}

}