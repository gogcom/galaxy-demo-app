#ifndef GOGTRON_MESSAGE_GAME_TICK_H
#define GOGTRON_MESSAGE_GAME_TICK_H

#include <engine/serialize/JsonSerializable.h>
#include <game/Player.h>

namespace gogtron
{
    namespace message
    {

        class GameTick : public serialize::JsonSerializable
        {
        public:

            GameTick() = default;
            GameTick(const std::vector<PlayerPtr>& players);

            virtual bool Serialize(Json::Value& root);
            virtual bool Deserialize(Json::Value& root);

            const std::vector<PlayerPtr>& GetPlayers() const;

        private:

            std::vector<PlayerPtr> players;
        };

    }
}

#endif