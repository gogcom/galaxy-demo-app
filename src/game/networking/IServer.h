#ifndef GALAXY_DEMO_I_SERVER_H
#define GALAXY_DEMO_I_SERVER_H

#include <game/Player.h>
#include <engine/networking/AbstractServer.h>
#include <engine/gamelogic/GameLogic.h>
#include <memory>
#include <string>
#include <vector>
#include <vector>
#include <map>
#include <utility>

namespace galaxy::demo
{

	class IGame;
	using IGamePtr = std::shared_ptr<IGame>;

	namespace networking
	{

		class IServer : public networking::AbstractServer
		{
		public:

			IServer(const IGamePtr& _game)
				: game(_game)
			{
			}

			virtual ~IServer()
			{
				gameLogic.reset();
			}

			virtual bool RetrievedReadyFromEachClient() const = 0;
			virtual bool SendInitGame() = 0;
			virtual bool SendGameTick(const std::vector<PlayerPtr>& players) = 0;
			virtual bool SendGameResults(const std::vector<PlayerPtr>& players, uint64_t gameTime) = 0;

			void SetPlayers(const std::vector<PlayerPtr>& _players)
			{
				players = _players;
			}

			void AddPlayer(const PlayerPtr& player)
			{
				players.push_back(player);
			}

			const std::vector<PlayerPtr>& GetPlayers() const
			{
				return players;
			}

			void InitGameLogic()
			{
				gameLogic = std::make_unique<gamelogic::GameLogic>(GetPlayers());
			}

			gamelogic::GameLogic& GetGameLogic()
			{
				return *gameLogic;
			}

		protected:

			virtual void OnServerP2PPacketAvailable(uint32_t msgSize, uint8_t channel) = 0;

			IGamePtr game;
			std::unique_ptr<gamelogic::GameLogic> gameLogic;
			std::vector<PlayerPtr> players;
			std::map<galaxy::api::GalaxyID, std::vector<std::string>> messages;
		};

		using IServerPtr = std::shared_ptr<IServer>;

	}
}

#endif