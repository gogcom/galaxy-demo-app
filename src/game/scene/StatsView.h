#ifndef GALAXY_DEMO_SCENE_STATS_VIEW_H
#define GALAXY_DEMO_SCENE_STATS_VIEW_H

#include "GameState.h"
#include <engine/system/GUIElement.h>
#include <vector>

namespace galaxy::demo
{
	namespace scene
	{

		class StatsView : public GameState
		{
		public:

			StatsView(const IGamePtr& _game);

			virtual bool Init() override;
			virtual bool Release() override;

			virtual void OnMouseDown(std::uint32_t x, std::uint32_t y) override;
			virtual void OnMouseMotion(std::uint32_t x, std::uint32_t y) override;
			virtual void OnKeyDown(SDL_Keysym key) override;
			virtual void OnLobbyEvent(const networking::LobbyEvent& lobbyEvent) override;

			virtual bool Update() override;
			virtual bool Display(const renderer::OGLRendererPtr& renderEngine) override;

		private:

			std::string errorMessage;
			std::vector<system::GUIElementPtr> guiElements;
		};

	}
}

#endif