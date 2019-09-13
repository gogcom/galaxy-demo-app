#ifndef GALAXY_DEMO_SCENE_START_MENU_H
#define GALAXY_DEMO_SCENE_START_MENU_H

#include "GameState.h"
#include <engine/system/GUIElement.h>
#include <vector>

namespace galaxy::demo
{
	namespace scene
	{

		enum class GalaxyStatus
		{
			NotInitialized  = 0b0001, // Galaxy Peer has not been fully initialized
			SignedOff       = 0b0010, // Galaxy Peer initialized, but user is not yet authenticated
			SignedInOffline = 0b0100, // User has been successfully signed in locally to Galaxy Service, but not yet logged on to Galaxy backend services
			SignedInOnline  = 0b1000, // User successfully authorized on Galaxy backend services
			Any             = 0b1111
		};

		inline GalaxyStatus operator|(GalaxyStatus a, GalaxyStatus b)
		{
			return static_cast<GalaxyStatus>(static_cast<int>(a) | static_cast<int>(b));
		}

		inline bool operator&(GalaxyStatus a, GalaxyStatus b)
		{
			return (static_cast<int>(a) & static_cast<int>(b)) != 0;
		}

		class StartMenu : public GameState
		{
		public:

			StartMenu(const IGamePtr& _game);

			virtual bool Init() override;
			virtual bool Release() override;

			virtual void OnMouseDown(std::uint32_t x, std::uint32_t y) override;
			virtual void OnMouseMotion(std::uint32_t x, std::uint32_t y) override;
			virtual void OnKeyDown(SDL_Keysym key) override;
			virtual void OnLobbyEvent(const networking::LobbyEvent& lobbyEvent) override;

			virtual bool Update() override;
			virtual bool Display(const renderer::OGLRendererPtr& renderEngine) override;

		private:

			std::vector<std::pair<system::GUIElementPtr, GalaxyStatus>> guiElements;
			std::string dlcStatus{"DLC not installed."};
		};

	}
}

#endif