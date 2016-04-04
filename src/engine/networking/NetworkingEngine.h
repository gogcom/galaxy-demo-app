#ifndef GOGTRON_NETWORKING_NETWORKING_ENGINE_H
#define GOGTRON_NETWORKING_NETWORKING_ENGINE_H

#include <galaxy/GalaxyApi.h>
#include <string>

namespace gogtron
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