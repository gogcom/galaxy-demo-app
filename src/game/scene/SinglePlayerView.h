#ifndef GOGTRON_SCENE_SINGLE_PLAYER_VIEW_H
#define GOGTRON_SCENE_SINGLE_PLAYER_VIEW_H

#include "GameState.h"
#include <engine/system/GUIElement.h>
#include <galaxy/GalaxyApi.h>
#include <vector>

namespace gogtron
{
	namespace scene
	{

		class SinglePlayerView : public GameState
		{
		public:

			SinglePlayerView(const IGamePtr& _game);

			virtual bool Init();
			virtual bool Release();

			virtual void OnMouseDown(std::uint32_t x, std::uint32_t y) override;
			virtual void OnMouseMotion(std::uint32_t x, std::uint32_t y) override;
			virtual void OnKeyDown(SDL_Keysym key) override;
			virtual void OnLobbyEvent(const networking::LobbyEvent& lobbyEvent) override;

			virtual bool Update() override;
			virtual bool Display(const renderer::OGLRendererPtr& renderEngine) override;

		private:

			system::GUIElementPtr backButton;
			std::vector<std::pair<system::GUIElementPtr, std::string>> guiElements;
		};

	}
}

#endif