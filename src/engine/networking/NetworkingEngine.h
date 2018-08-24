#ifndef GALAXY_DEMO_NETWORKING_NETWORKING_ENGINE_H
#define GALAXY_DEMO_NETWORKING_NETWORKING_ENGINE_H

#include <galaxy/GalaxyApi.h>
#include <string>

namespace galaxy::demo
{
	namespace networking
	{

		class NetworkingEngine
		{
		public:

			static bool SendMessage(const std::string& message, const galaxy::api::GalaxyID& userID);
		};

	}
}

#endif