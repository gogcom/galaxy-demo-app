#ifndef GALAXY_DEMO_SERVER_H
#define GALAXY_DEMO_SERVER_H

#include "IServer.h"

namespace galaxy::demo
{
	namespace networking
	{

		class Server : public IServer
		{
		public:

			Server(const IGamePtr& game);

			virtual bool SendMessageToAll(const std::string& message) const override;

			virtual bool RetrievedReadyFromEachClient() const override;
			virtual bool SendInitGame() override;
			virtual bool SendGameTick(const std::vector<PlayerPtr>& players) override;
			virtual bool SendGameResults(const std::vector<PlayerPtr>& players, uint64_t gameTime) override;

		private:

			virtual void OnP2PPacketAvailable(uint32_t msgSize, uint8_t channel) override;

			bool CreateInitGame(std::string& message);
		};

	}
}

#endif