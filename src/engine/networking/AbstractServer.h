#ifndef GALAXY_DEMO_NETWORKING_ABSTRACT_SERVER_H
#define GALAXY_DEMO_NETWORKING_ABSTRACT_SERVER_H

#include <galaxy/GalaxyApi.h>
#include <string>
#include <vector>

namespace galaxy::demo
{
	namespace networking
	{

		class AbstractServer : public galaxy::api::GlobalServerNetworkingListener
		{
		public:

			virtual ~AbstractServer() = 0;

			virtual bool SendMessageToAll(const std::string& message) const;

			void SetLobbyMembers(const std::vector<galaxy::api::GalaxyID>& members);
			void AddLobbyMember(const galaxy::api::GalaxyID& member);
			const std::vector<galaxy::api::GalaxyID>& GetLobbyMembers() const;

		protected:

			std::vector<galaxy::api::GalaxyID> lobbyMembers;
		};

	}
}

#endif