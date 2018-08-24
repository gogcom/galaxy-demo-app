#include "Ready.h"

namespace galaxy::demo
{
	namespace message
	{

		std::string Ready::type = "Ready";

		Ready::Ready(const galaxy::api::GalaxyID& _playerID)
			: playerID(_playerID)
		{
		}

		bool Ready::Serialize(Json::Value& root)
		{
			root["type"] = Ready::type;
			root["data"]["playerID"] = playerID.ToUint64();
			return true;
		}

		bool Ready::Deserialize(Json::Value& root)
		{
			if (!root.isMember("data"))
			{
				return false;
			}

			Json::Value data = root["data"];

			if (!data.isMember("playerID"))
			{
				return false;
			}

			playerID = data.get("playerID", 0).asUInt64();
			return true;
		}

		const galaxy::api::GalaxyID& Ready::GetPlayerID() const
		{
			return playerID;
		}

		void Ready::SetPlayerID(const galaxy::api::GalaxyID& _playerID)
		{
			playerID = _playerID;
		}

	}
}

