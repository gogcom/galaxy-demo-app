#include "GameContext.h"
#include <cassert>

namespace game
{

	GameContext::GameContext()
		: gameState(GameContext::NOT_AUTHORIZED)
	{
	}

	GameContext::operator const GameContext::GameState &() const
	{
		return gameState;
	}

	void GameContext::SetGameState(GameContext::GameState _gameState)
	{
		gameState = _gameState;
	}

	const galaxy::api::GalaxyID& GameContext::GetUserID() const
	{
		return userID;
	}

	void GameContext::SetUserID(const galaxy::api::GalaxyID& _userID)
	{
		userID = _userID;
	}

	const galaxy::api::GalaxyID& GameContext::GetLobbyID() const
	{
		return lobbyID;
	}

	void GameContext::SetLobbyID(const galaxy::api::GalaxyID& _lobbyID)
	{
		lobbyID = _lobbyID;
	}

	const GameplayData& GameContext::GetGameplayData() const
	{
		return gameplayData;
	}

	GameplayData& GameContext::GetGameplayData()
	{
		return gameplayData;
	}

	void GameContext::PrintGameControls() const
	{
		fprintf(stdout, "Press 'h' to print available options\n");
		fprintf(stdout, "Press 'q' to quit\n");
		fprintf(stdout, "Press 's' to send a lobby message (only while in lobby)\n");
		fprintf(stdout, "Press 'k' to send a packet to lobby user (only while in lobby)\n");
		fprintf(stdout, "Press 'p' to send a packet to lobby owner (only while in lobby)\n");
		fprintf(stdout, "Press 'm' to set lobby member data (only while in lobby)\n");
		fprintf(stdout, "Press 'n' to set lobby data (only while in lobby and if lobby owner)\n");
		fprintf(stdout, "Press 'r' to request user stats&achievements\n");
		fprintf(stdout, "Press 'u' to unlock achievements\n");
		fprintf(stdout, "Press 'c' to clear achievements\n");
		fprintf(stdout, "Press 't' to store stats&achievements\n");
		fprintf(stdout, "Press 'i' to set int statistic\n");
		fprintf(stdout, "Press 'f' to set float statistic\n");
		fprintf(stdout, "Press 'a' to set avg statistic\n");
		fprintf(stdout, "Press 'l' to request leaderboards\n");
		fprintf(stdout, "Press 'g' to request leaderboard entires global\n");
		fprintf(stdout, "Press 'd' to request leaderboard entires around user\n");
		fprintf(stdout, "Press 'o' to request leaderboard entires for users\n");
		fprintf(stdout, "Press 'e' to set leaderboard score\n");
	}

}