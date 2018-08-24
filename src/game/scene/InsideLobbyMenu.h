#ifndef GALAXY_DEMO_SCENE_INSIDE_LOBBY_MENU_H
#define GALAXY_DEMO_SCENE_INSIDE_LOBBY_MENU_H

#include "GameState.h"
#include <engine/system/GUIElement.h>
#include <galaxy/GalaxyApi.h>
#include <vector>

namespace galaxy::demo
{
	namespace scene
	{

		class InsideLobbyMenu : public GameState
		{
		public:

			InsideLobbyMenu(const IGamePtr& _game);

			virtual bool Init() override;
			virtual bool Release() override;

			virtual void OnMouseDown(std::uint32_t x, std::uint32_t y) override;
			virtual void OnMouseMotion(std::uint32_t x, std::uint32_t y) override;
			virtual void OnKeyDown(SDL_Keysym key) override;
			virtual void OnLobbyEvent(const networking::LobbyEvent& lobbyEvent) override;

			virtual bool Update() override;
			virtual bool Display(const renderer::OGLRendererPtr& renderEngine) override;

		private:

			std::vector<system::GUIElementPtr> guiElements;
		};

	}
}

#endif