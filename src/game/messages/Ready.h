#ifndef GALAXY_DEMO_MESSAGE_READY_H
#define GALAXY_DEMO_MESSAGE_READY_H

#include <engine/serialize/JsonSerializable.h>
#include <galaxy/GalaxyApi.h>
#include <string>

namespace galaxy::demo
{
	namespace message
	{

		class Ready : public serialize::JsonSerializable
		{
		public:

			Ready() = default;
			Ready(const galaxy::api::GalaxyID& playerID);
			virtual bool Serialize(Json::Value& root);
			virtual bool Deserialize(Json::Value& root);

			const galaxy::api::GalaxyID& GetPlayerID() const;

			void SetPlayerID(const galaxy::api::GalaxyID& playerID);

		private:

			galaxy::api::GalaxyID playerID;
			static std::string type;
		};

	}
}

#endif