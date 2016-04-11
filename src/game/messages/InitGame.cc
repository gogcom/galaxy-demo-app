#include "InitGame.h"

namespace gogtron
{
    namespace message
    {

        InitGame::InitGame()
        {
        }

        InitGame::InitGame(const std::vector<PlayerPtr>& _players)
            : players(_players)
        {
        }

        bool InitGame::Serialize(Json::Value& root)
        {
            root["type"] = "InitGame";

            for (auto&& player : players)
            {
                Json::Value data;
                data["id"] = player->GetGalaxyID().ToUint64();
                data["color"]["x"] = player->GetColor().x;
                data["color"]["y"] = player->GetColor().y;
                data["color"]["z"] = player->GetColor().z;
                data["color"]["w"] = player->GetColor().w;
                data["direction"]["x"] = player->GetDirection().x;
                data["direction"]["y"] = player->GetDirection().y;
                data["position"]["x"] = player->GetPosition().x;
                data["position"]["y"] = player->GetPosition().y;
                data["position"]["z"] = player->GetPosition().z;
                root["data"]["players"].append(data);
            }
            return true;
        }

        bool InitGame::Deserialize(Json::Value& root)
        {
            if (!root.isMember("data"))
            {
                return false;
            }

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

                Json::Value jsonPlayerColor = jsonPlayer["color"];

                glm::vec4 color;
                color.x = jsonPlayerColor.get("x", 0).asFloat();
                color.y = jsonPlayerColor.get("y", 0).asFloat();
                color.z = jsonPlayerColor.get("z", 0).asFloat();
                color.w = jsonPlayerColor.get("w", 0).asFloat();
                player->SetColor(color);

                Json::Value jsonPlayerDirection = jsonPlayer["direction"];

                glm::vec2 direction;
                direction.x = jsonPlayerDirection.get("x", 0).asFloat();
                direction.y = jsonPlayerDirection.get("y", 0).asFloat();
                player->SetDirection(direction);

                Json::Value jsonPlayerPosition = jsonPlayer["position"];

                glm::vec3 position;
                position.x = jsonPlayerPosition.get("x", 0).asFloat();
                position.y = jsonPlayerPosition.get("y", 0).asFloat();
                position.z = jsonPlayerPosition.get("z", 0).asFloat();
                player->SetPosition(position);

                players.push_back(player);
            }

            return true;
        }

        const std::vector <PlayerPtr>& InitGame::GetPlayers() const
        {
            return players;
        }

    }
}

