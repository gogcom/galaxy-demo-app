#ifndef GALAXY_DEMO_I_GAME_H
#define GALAXY_DEMO_I_GAME_H

#include "GameManager.h"
#include "GameplayData.h"
#include <game/scene/GameState.h>
#include <game/networking/ILobby.h>
#include <game/networking/LobbyEvent.h>
#include <game/networking/IServer.h>
#include <game/networking/IClient.h>
#include <memory>
#include <cstdint>
#include <unordered_map>

namespace galaxy::demo
{

	class IGame
	{
	public:

		IGame()
			: closeRequested(false)
			, currentGameState(scene::GameState::State::INIT_FAILED_VIEW)
		{
		}

		virtual ~IGame() = default;

		virtual bool Init(int argc, char** argv) = 0;
		virtual bool Release() = 0;
		virtual bool Update() = 0;

		virtual void OnMouseDown(std::uint32_t x, std::uint32_t y) = 0;
		virtual void OnMouseMotion(std::uint32_t x, std::uint32_t y) = 0;
		virtual void OnKeyDown(SDL_Keysym key) = 0;
		virtual void OnLobbyEvent(const networking::LobbyEvent& lobbyEvent) = 0;

		scene::GameState::State GetGameState() const
		{
			return currentGameState;
		}
		virtual bool SetGameState(const scene::GameState::State& state) = 0;
		void SetLobby(const networking::ILobbyPtr& _lobby)
		{
			lobby = _lobby;
		}
		const networking::ILobbyPtr& GetLobby() const
		{
			return lobby;
		}

		void SetServer(const networking::IServerPtr& _server)
		{
			server = _server;
		}
		const networking::IServerPtr& GetServer() const
		{
			return server;
		}

		void SetClient(const networking::IClientPtr& _client)
		{
			client = _client;
		}
		const networking::IClientPtr& GetClient() const
		{
			return client;
		}

		GameManager& GetGameManager()
		{
			return gameManager;
		}

		GameplayData& GetGameplayData()
		{
			return gameplayData;
		}

		void Close()
		{
			closeRequested = true;
		}
		bool IsRunning() const
		{
			return !closeRequested;
		}

	protected:

		bool closeRequested;
		std::unordered_map<scene::GameState::State, scene::GameStatePtr> gameStates;
		scene::GameStatePtr gameState;
		scene::GameState::State currentGameState;
		networking::ILobbyPtr lobby;
		networking::IServerPtr server;
		networking::IClientPtr client;
		GameManager gameManager;
		GameplayData gameplayData;
	};

	using IGamePtr = std::shared_ptr<IGame>;

}

#endif