#ifndef GOGTRON_NETWORKING_ABSTRACT_CLIENT_H
#define GOGTRON_NETWORKING_ABSTRACT_CLIENT_H

#include <galaxy/GalaxyApi.h>
#include <string>

namespace gogtron
{
	namespace networking
	{

		class AbstractClient : public galaxy::api::GlobalNetworkingListener
		{
		public:

			virtual ~AbstractClient() = 0;

			bool SendMessage(const std::string& message, const galaxy::api::GalaxyID& userID);

			bool SendMessageToServer(const std::string& message);

			const galaxy::api::GalaxyID& GetServerID() const;
			void SetServerID(const galaxy::api::GalaxyID& _serverID);

		protected:

			galaxy::api::GalaxyID serverID;
		};

	}
}

#endif