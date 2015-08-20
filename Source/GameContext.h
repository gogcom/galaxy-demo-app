#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "GameplayData.h"
#include <galaxy/GalaxyID.h>
#include <set>

namespace game
{

	class GameContext
	{
	public:

		enum GameState
		{
			NOT_AUTHORIZED,
			IN_LOBBY,
			QUIT
		};

		GameContext();

		operator const GameState&() const;
		void SetGameState(GameState gameState);

		const galaxy::api::GalaxyID& GetUserID() const;
		void SetUserID(const galaxy::api::GalaxyID& userID);

		const galaxy::api::GalaxyID& GetLobbyID() const;
		void SetLobbyID(const galaxy::api::GalaxyID& lobbyID);

		const GameplayData& GetGameplayData() const;
		GameplayData& GetGameplayData();

		const std::set<galaxy::api::GalaxyID>& GetLobbies() const;
		void SetLobbies(const std::set<galaxy::api::GalaxyID>& lobbies);

		void PrintGameControls() const;

	private:

		GameState gameState;
		galaxy::api::GalaxyID userID;
		galaxy::api::GalaxyID lobbyID;
		GameplayData gameplayData;
		std::set<galaxy::api::GalaxyID> lobbies;
	};

}

#endif