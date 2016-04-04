#ifndef GOGTRON_MESSAGE_GAME_RESULTS_H
#define GOGTRON_MESSAGE_GAME_RESULTS_H

#include <engine/serialize/JsonSerializable.h>
#include <game/Player.h>

namespace gogtron
{
    namespace message
    {

        class GameResults : public serialize::JsonSerializable
        {
        public:

            GameResults();
            GameResults(const std::vector<PlayerPtr>& players, int gameTimeSeconds);

            virtual bool Serialize(Json::Value& root);
            virtual bool Deserialize(Json::Value& root);

            const std::vector<PlayerPtr>& GetPlayers() const;
            int GetGameTime() const;

        private:

            std::vector<PlayerPtr> players;
            int gameTimeSeconds;
        };

    }
}

#endif