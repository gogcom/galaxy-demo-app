#ifndef GOGTRON_MESSAGE_UPDATE_DIRECTION_H
#define GOGTRON_MESSAGE_UPDATE_DIRECTION_H

#include <engine/serialize/JsonSerializable.h>
#include <glm/vec2.hpp>
#include <string>

namespace gogtron
{
    namespace message
    {

        class UpdateDirection : public serialize::JsonSerializable
        {
        public:

            UpdateDirection();
            UpdateDirection(const glm::vec2& direction);

            virtual bool Serialize(Json::Value& root);
            virtual bool Deserialize(Json::Value& root);

            const glm::vec2& GetDirection() const;

        private:

            glm::vec2 direction;
            static std::string type;
        };

    }
}

#endif