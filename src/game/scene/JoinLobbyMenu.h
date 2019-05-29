#ifndef GALAXY_DEMO_SCENE_JOIN_LOBBY_MENU_H
#define GALAXY_DEMO_SCENE_JOIN_LOBBY_MENU_H

#include "GameState.h"
#include <engine/system/GUIElement.h>
#include <galaxy/GalaxyApi.h>
#include <vector>

namespace galaxy::demo
{
	namespace scene
	{

		class JoinLobbyMenu
			: public GameState
			, public galaxy::api::GlobalLobbyListListener
		{
		public:

			JoinLobbyMenu(const IGamePtr& _game);

			 bool Init() override;
			 bool Release() override;

			 void OnMouseDown(std::uint32_t x, std::uint32_t y) override;
			 void OnMouseMotion(std::uint32_t x, std::uint32_t y) override;
			 void OnKeyDown(SDL_Keysym key) override;
			 void OnLobbyEvent(const networking::LobbyEvent& lobbyEvent) override;

			 bool Update() override;
			 bool Display(const renderer::OGLRendererPtr& renderEngine) override;

		private:

			 void OnLobbyList(uint32_t lobbyCount, galaxy::api::LobbyListResult result) override;

			system::GUIElementPtr backButton;
			std::vector<system::GUIElementPtr> guiElements;
			bool anyLobbies;
		};

	}
}

#endif