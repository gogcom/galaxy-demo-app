#ifndef GOGTRON_SCENE_REMOTE_STORAGE_VIEW_H
#define GOGTRON_SCENE_REMOTE_STORAGE_VIEW_H

#include "GameState.h"
#include <engine/system/GUIElement.h>
#include <vector>

namespace gogtron
{
	namespace scene
	{

		class RemoteStorageView : public GameState
		{
		public:

			RemoteStorageView(const IGamePtr& _game);

			virtual bool Init();
			virtual bool Release();

			virtual void OnMouseDown(std::uint32_t x, std::uint32_t y) override;
			virtual void OnMouseMotion(std::uint32_t x, std::uint32_t y) override;
			virtual void OnKeyDown(SDL_Keysym key) override;
			virtual void OnLobbyEvent(const networking::LobbyEvent& lobbyEvent) override;

			virtual bool Update() override;
			virtual bool Display(const renderer::OGLRendererPtr& renderEngine) override;

		private:

			void Reset();

			bool synchronizationRequested;
			bool fileLoaded;
			std::string errorMessage;
			std::string currentFileContent;
			std::string newFileContent;
			std::vector<system::GUIElementPtr> guiElements;
		};

	}
}

#endif