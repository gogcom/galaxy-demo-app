#include "UpdateDirection.h"

namespace gogtron
{
    namespace message
    {

        std::string UpdateDirection::type = "UpdateDirection";

        UpdateDirection::UpdateDirection()
        {
        }

        UpdateDirection::UpdateDirection(const glm::vec2& _direction)
            : direction(_direction)
        {

        }

        bool UpdateDirection::Serialize(Json::Value& root)
        {
            root["type"] = UpdateDirection::type;
            root["data"]["direction"]["x"] = direction.x;
            root["data"]["direction"]["y"] = direction.y;
            return true;
        }

        bool UpdateDirection::Deserialize(Json::Value& root)
        {
            if (!root.isMember("data"))
            {
                return false;
            }

            Json::Value data = root["data"];

            if (!data.isMember("direction"))
            {
                return false;
            }

            Json::Value jsonDirection = data["direction"];
            direction.x = jsonDirection.get("x", 0).asFloat();
            direction.y = jsonDirection.get("y", 0).asFloat();
            return true;
        }

        const glm::vec2& UpdateDirection::GetDirection() const
        {
            return direction;
        }

    }
}

