#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#include <map>
#include <list>
#include <stdint.h>

namespace game
{

	class GameContext;

	class ICommand
	{
	public:

		virtual ~ICommand() {}
		virtual void Execute(GameContext& gameContext) = 0;
	};

	class HelpCommand : public ICommand
	{
	public:

		virtual void Execute(GameContext& gameContext);
	};

	class QuitCommand : public ICommand
	{
	public:

		virtual void Execute(GameContext& gameContext);
	};

	class SendLobbyMessageCommand : public ICommand
	{
	public:

		virtual void Execute(GameContext& gameContext);
	};

	class SendPacketToLobbyUserCommand : public ICommand
	{
	public:

		void Execute(GameContext& gameContext);
	};

	class SendPacketToLobbyOwnerCommand : public ICommand
	{
	public:

		virtual void Execute(GameContext& gameContext);
	};

	class SetLobbyMemberDataCommand : public ICommand
	{
	public:

		virtual void Execute(GameContext& gameContext);
	};

	class SetLobbyDataCommand : public ICommand
	{
	public:

		virtual void Execute(GameContext& gameContext);
	};

	class RequestUserStatsAndAchievementsCommand : public ICommand
	{
	public:

		virtual void Execute(GameContext& gameContext);
	};

	class UnlockAchievementCommand : public ICommand
	{
	public:

		virtual void Execute(GameContext& gameContext);
	};

	class ClearAchievementCommand : public ICommand
	{
	public:

		virtual void Execute(GameContext& gameContext);
	};

	class SetIntStatisticCommand : public ICommand
	{
	public:

		virtual void Execute(GameContext& gameContext);
	};

	class SetFloatStatisticCommand : public ICommand
	{
	public:

		virtual void Execute(GameContext& gameContext);
	};

	class SetAvgStatisticCommand : public ICommand
	{
	public:

		virtual void Execute(GameContext& gameContext);
	};

	class StoreStatsAndAchievementsCommand : public ICommand
	{
	public:

		virtual void Execute(GameContext& gameContext);
	};

	class RequestLeaderboardsCommand : public ICommand
	{
	public:

		virtual void Execute(GameContext& gameContext);
	};

	class RequestLeaderboardEntriesGlobalCommnad : public ICommand
	{
	public:

		virtual void Execute(GameContext& gameContext);
	};

	class RequestLeaderboardEntriesAroundUserCommand : public ICommand
	{
	public:

		virtual void Execute(GameContext& gameContext);
	};

	class RequestLeaderboardEntriesForUsersCommand : public ICommand
	{
	public:

		virtual void Execute(GameContext& gameContext);
	};

	class SetLeaderboardScoreCommand : public ICommand
	{
	public:

		virtual void Execute(GameContext& gameContext);
	};

	class InputHandler
	{
	public:

		InputHandler();

		~InputHandler();

		void ReadCommands(std::list<ICommand*>& commands);

		char ReadKey() const;

	private:

		std::map<const char, ICommand*> registry;
	};

}

#endif /* INPUT_H */