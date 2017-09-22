#ifndef GOGTRON_SCENE_START_MENU_H
#define GOGTRON_SCENE_START_MENU_H

#include "GameState.h"
#include <engine/system/GUIElement.h>
#include <vector>

namespace gogtron
{
	namespace scene
	{

		enum class GalaxyStatus
		{
			NotInitialized = 0b001,
			Offline        = 0b010,
			SignedIn       = 0b100,
		};

		inline GalaxyStatus operator|(GalaxyStatus a, GalaxyStatus b)
		{
			return static_cast<GalaxyStatus>(static_cast<int>(a) | static_cast<int>(b));
		}

		inline bool operator&(GalaxyStatus a, GalaxyStatus b)
		{
			auto aa = static_cast<int>(a);
			auto bb = static_cast<int>(b);
			auto r = a | b;
			return (static_cast<int>(a) & static_cast<int>(b)) != 0;
		}

		class StartMenu : public GameState
		{
		public:

			StartMenu(const IGamePtr& _game);

			virtual bool Init();
			virtual bool Release();

			virtual void OnMouseDown(std::uint32_t x, std::uint32_t y) override;
			virtual void OnMouseMotion(std::uint32_t x, std::uint32_t y) override;
			virtual void OnKeyDown(SDL_Keysym key) override;
			virtual void OnLobbyEvent(const networking::LobbyEvent& lobbyEvent) override;

			virtual bool Update() override;
			virtual bool Display(const renderer::OGLRendererPtr& renderEngine) override;

		private:

			std::vector<std::pair<system::GUIElementPtr, GalaxyStatus>> guiElements;
		};

	}
}

#endif