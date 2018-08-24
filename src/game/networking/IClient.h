#ifndef GALAXY_DEMO_NETWORKING_I_CLIENT_H
#define GALAXY_DEMO_NETWORKING_I_CLIENT_H

#include <engine/networking/AbstractClient.h>
#include <glm/vec2.hpp>
#include <memory>
#include <string>
#include <vector>

namespace galaxy::demo
{

	class IGame;
	using IGamePtr = std::shared_ptr<IGame>;

	namespace networking
	{

		class IClient : public networking::AbstractClient
		{
		public:

			IClient(const IGamePtr& _game)
				: game(_game)
			{
			}

			virtual ~IClient() = default;

			virtual bool SendReadyToServer() = 0;
			virtual bool SendUpdateDirectionToServer(const glm::vec2& direction) = 0;

		protected:

			IGamePtr game;
			std::vector<std::string> messages;
		};

		using IClientPtr = std::shared_ptr < IClient >;

	}
}

#endif