#ifndef GALAXY_DEMO_SCENE_CLOUD_STORAGE_VIEW_H
#define GALAXY_DEMO_SCENE_CLOUD_STORAGE_VIEW_H

#include "GameState.h"
#include <engine/system/GUIElement.h>

#include <galaxy/IStorage.h>

#include <vector>

namespace galaxy::demo
{
	namespace scene
	{

		class CloudStorageView
			: public GameState
		{
		public:

			explicit CloudStorageView(const IGamePtr& _game);

			bool Init() override;
			bool Release() override;

			void OnMouseDown(std::uint32_t x, std::uint32_t y) override;
			void OnMouseMotion(std::uint32_t x, std::uint32_t y) override;
			void OnKeyDown(SDL_Keysym key) override;
			void OnLobbyEvent(const networking::LobbyEvent& lobbyEvent) override;

			bool Update() override;
			bool Display(const renderer::OGLRendererPtr& renderEngine) override;

		private:

			void SaveNewFile() const;
			uint32_t ReadSavedFiles();

			std::string newFileName;

			std::vector<std::string> savedFileNames;
			std::vector<system::GUIElementPtr> guiElements;
			std::vector<system::GUIElementPtr> fileRemoveButtons;
		};

	}
}

#endif