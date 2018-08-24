#ifndef GALAXY_DEMO_MESSAGE_GAME_TICK_H
#define GALAXY_DEMO_MESSAGE_GAME_TICK_H

#include <engine/serialize/JsonSerializable.h>
#include <game/Player.h>

namespace galaxy::demo
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