#ifndef GALAXY_DEMO_MESSAGE_GAME_RESULTS_H
#define GALAXY_DEMO_MESSAGE_GAME_RESULTS_H

#include <engine/serialize/JsonSerializable.h>
#include <game/Player.h>

namespace galaxy::demo
{
	namespace message
	{

		class GameResults : public serialize::JsonSerializable
		{
		public:

			GameResults();
			GameResults(const std::vector<PlayerPtr>& players, uint64_t gameTimeSeconds);

			virtual bool Serialize(Json::Value& root);
			virtual bool Deserialize(Json::Value& root);

			const std::vector<PlayerPtr>& GetPlayers() const;
			uint64_t GetGameTime() const;

		private:

			std::vector<PlayerPtr> players;
			uint64_t gameTimeSeconds;
		};

	}
}

#endif