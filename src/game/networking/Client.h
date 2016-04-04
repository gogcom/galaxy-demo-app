#ifndef GOGTRON_NETWORKING_CLIENT_H
#define GOGTRON_NETWORKING_CLIENT_H

#include "IClient.h"

namespace gogtron
{
	namespace networking
	{

		class Client : public IClient
		{
		public:

			Client(const IGamePtr& game);

			virtual bool SendReadyToServer() override;
			virtual bool SendUpdateDirectionToServer(const glm::vec2& direction) override;

		private:

			virtual void OnP2PPacketAvailable(uint32_t msgSize, uint8_t channel) override;
		};

	}
}

#endif