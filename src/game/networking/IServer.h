#ifndef GOGTRON_I_SERVER_H
#define GOGTRON_I_SERVER_H

#include <game/Player.h>
#include <engine/networking/AbstractServer.h>
#include <memory>
#include <string>
#include <vector>
#include <vector>
#include <map>
#include <utility>

namespace gogtron
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

			virtual ~IServer() = default;

			virtual bool RetrievedReadyFromEachClient() const = 0;
			virtual bool SendInitGame() = 0;
			virtual bool SendGameTick(const std::vector<PlayerPtr>& players) = 0;
			virtual bool SendGameResults(const std::vector<PlayerPtr>& players, int gameTime) = 0;

			void SetPlayers(const std::vector<PlayerPtr>& _players) { players = _players; }
			void AddPlayer(const PlayerPtr& player) { players.push_back(player); }
			const std::vector<PlayerPtr>& GetPlayers() const { return players; }

		protected:

			virtual void OnServerP2PPacketAvailable(uint32_t msgSize, uint8_t channel) = 0;

			IGamePtr game;
			std::vector<PlayerPtr> players;
			std::map<galaxy::api::GalaxyID, std::vector<std::string>> messages;
		};

		using IServerPtr = std::shared_ptr<IServer>;

	}
}

#endif