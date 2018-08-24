#ifndef GALAXY_DEMO_MESSAGE_INIT_GAME_H
#define GALAXY_DEMO_MESSAGE_INIT_GAME_H

#include <engine/serialize/JsonSerializable.h>
#include <game/Player.h>

namespace galaxy::demo
{
	namespace message
	{

		class InitGame : public serialize::JsonSerializable
		{
		public:

			InitGame();
			InitGame(const std::vector<PlayerPtr>& players);

			virtual bool Serialize(Json::Value& root);
			virtual bool Deserialize(Json::Value& root);

			const std::vector<PlayerPtr>& GetPlayers() const;

		private:

			std::vector<PlayerPtr> players;
		};

	}
}

#endif