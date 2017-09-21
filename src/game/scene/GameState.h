#ifndef GOGTRON_SCENE_GAME_STATE_H
#define GOGTRON_SCENE_GAME_STATE_H

#include <game/networking/LobbyEvent.h>
#include <engine/renderer/OGLRenderer.h>

namespace gogtron
{

	class IGame;
	using IGamePtr = std::shared_ptr<IGame>;

	namespace scene
	{

		class GameState
		{
		public:

			enum class State
			{
				INIT_FAILED_VIEW,
				START_MENU,
				STATS_VIEW,
				SINGLE_PLAYER_VIEW,
				LEADERBOARDS_VIEW,
				CLOUD_STORAGE_VIEW,
				LOBBY_MENU,
				JOIN_LOBBY_MENU,
				IN_LOBBY_MENU,
				IN_GAME,
				GAME_RESULT
			};

			GameState(const IGamePtr& _game)
				: game(_game)
			{}

			virtual ~GameState() = default;

			virtual bool Init() = 0;
			virtual bool Release() = 0;

			virtual void OnMouseDown(std::uint32_t x, std::uint32_t y) = 0;
			virtual void OnMouseMotion(std::uint32_t x, std::uint32_t y) = 0;
			virtual void OnKeyDown(SDL_Keysym key) = 0;
			virtual void OnLobbyEvent(const networking::LobbyEvent& lobbyEvent) = 0;

			virtual bool Update() = 0;
			virtual bool Display(const renderer::OGLRendererPtr& renderEngine) = 0;

		protected:

			IGamePtr game;
		};

		using GameStatePtr = std::shared_ptr<GameState>;

	}
}

#endif