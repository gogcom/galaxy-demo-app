#ifndef GALAXY_DEMO_SCENE_GAME_STATE_H
#define GALAXY_DEMO_SCENE_GAME_STATE_H

#include <game/networking/LobbyEvent.h>
#include <engine/renderer/OGLRenderer.h>

#include <utility>

namespace galaxy::demo
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
				CHAT_VIEW,
				LOBBY_MENU,
				JOIN_LOBBY_MENU,
				IN_LOBBY_MENU,
				IN_GAME,
				GAME_RESULT
			};

			explicit GameState(IGamePtr  _game)
				: game(std::move(_game))
			{
				glViewport(0, 0, 1280, 720);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(0.0, 1280, 720, 1.0, -1.0, 1.0);

				glClearColor(0.0, 0.0, 0.0, 0.0);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(0.0, 1280, 720, 1.0, -1.0, 1.0);

				glEnable(GL_BLEND);
				glEnable(GL_TEXTURE_2D);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}

			virtual ~GameState() = default;

			virtual bool Init() = 0;
			virtual bool Release() = 0;

			virtual void OnMouseDown(std::uint32_t x, std::uint32_t y) = 0;
			virtual void OnMouseMotion(std::uint32_t x, std::uint32_t y) = 0;
			virtual void OnKeyDown(SDL_Keysym key) = 0;
			virtual void OnLobbyEvent(const networking::LobbyEvent& lobbyEvent) = 0;

			virtual bool Update() = 0;

			virtual bool Render(const renderer::OGLRendererPtr& renderEngine) final
			{
				renderEngine->StartScene();

				auto ret = Display(renderEngine);

				renderEngine->EndScene();
				return ret;
			}

		protected:

			virtual bool Display(const renderer::OGLRendererPtr& renderEngine) = 0;

			IGamePtr game;
		};

		using GameStatePtr = std::shared_ptr<GameState>;

	}
}

#endif