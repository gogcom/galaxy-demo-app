#ifndef GOGTRON_SCENE_GAME_H
#define GOGTRON_SCENE_GAME_H

#include "GameState.h"
#include <game/GameManager.h>
#include <engine/renderer/Camera.h>

namespace gogtron
{
	namespace scene
	{

		class Game : public GameState
		{
		public:

			Game(const IGamePtr& _game);

			virtual bool Init() override;
			virtual bool Release() override;

			virtual void OnMouseDown(std::uint32_t x, std::uint32_t y) override;
			virtual void OnMouseMotion(std::uint32_t x, std::uint32_t y) override;
			virtual void OnKeyDown(SDL_Keysym key) override;
			virtual void OnLobbyEvent(const networking::LobbyEvent& lobbyEvent) override;

			virtual bool Update() override;
			virtual bool Display(const renderer::OGLRendererPtr& renderEngine) override;

		private:

			void SendGameResults() const;

			GameManager gameManager;
			renderer::Camera camera;
			uint64_t startGameTime;
		};

	}
}

#endif