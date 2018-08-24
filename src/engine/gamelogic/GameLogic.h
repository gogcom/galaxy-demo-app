#pragma once

#include <game/Player.h>

namespace galaxy::demo::gamelogic
{

	class GameLogic final
	{
	public:

		GameLogic(PlayerPtrList players);

		void Tick();

		bool IsWinnerDefined() const;

	private:

		bool UpdatePlayersPositions();

		bool CheckPlayersCollisions();

		PlayerPtrList players;
	};

}