#include "GameResults.h"

namespace gogtron
{
	namespace message
	{

		GameResults::GameResults()
		{
		}

		GameResults::GameResults(const std::vector<PlayerPtr>& _players, uint64_t _gameTimeSeconds)
			: players(_players)
			, gameTimeSeconds(_gameTimeSeconds)
		{
		}

		bool GameResults::Serialize(Json::Value& root)
		{
			root["type"] = "GameResults";

			for (auto&& player : players)
			{
				Json::Value data;
				data["id"] = player->GetGalaxyID().ToUint64();
				data["points"] = player->GetPoints();
				root["data"]["players"].append(data);
			}

			root["time"] = gameTimeSeconds;
			return true;
		}

		bool GameResults::Deserialize(Json::Value& root)
		{
			if (!root.isMember("data"))
			{
				return false;
			}

			if (root.get("type", "").asString() != "GameResults")
			{
				return false;
			}

			gameTimeSeconds = root.get("time", 0).asInt();

			Json::Value data = root["data"];

			if (!data.isMember("players"))
			{
				return false;
			}

			Json::Value jsonPlayers = data["players"];

			if (!jsonPlayers.isArray())
			{
				return false;
			}

			for (std::uint32_t i = 0; i < jsonPlayers.size(); ++i)
			{
				PlayerPtr player = std::make_shared<Player>();

				Json::Value jsonPlayer = jsonPlayers[i];

				player->SetGalaxyID(jsonPlayer.get("id", 0).asInt64());
				player->SetPoints(jsonPlayer.get("points", 0).asInt());
				players.push_back(player);
			}

			return true;
		}

		const std::vector<PlayerPtr>& GameResults::GetPlayers() const
		{
			return players;
		}

		uint64_t GameResults::GetGameTime() const
		{
			return gameTimeSeconds;
		}

	}
}

