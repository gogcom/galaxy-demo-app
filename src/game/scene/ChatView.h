#ifndef GALAXY_DEMO_SCENE_CHAT_VIEW_H
#define GALAXY_DEMO_SCENE_CHAT_VIEW_H

#include "GameState.h"
#include <engine/system/GUIElement.h>
#include <game/networking/ChatRoom.h>
#include <galaxy/GalaxyApi.h>

#include <vector>

namespace galaxy::demo
{
	namespace scene
	{

		class ChatView
			: public GameState
			, public galaxy::api::IChatRoomWithUserRetrieveListener
			, public galaxy::api::GlobalFriendListListener
		{
		public:

			explicit ChatView(const IGamePtr& game);
			~ChatView() override;

			bool Init() override;
			bool Release() override;

			void OnMouseDown(std::uint32_t x, std::uint32_t y) override;
			void OnMouseMotion(std::uint32_t x, std::uint32_t y) override;
			void OnKeyDown(SDL_Keysym key) override;
			void OnLobbyEvent(const networking::LobbyEvent& lobbyEvent) override;

			bool Update() override;
			bool Display(const renderer::OGLRendererPtr& renderEngine) override;

		private:

			void OnChatRoomWithUserRetrieveSuccess(galaxy::api::GalaxyID galaxyID, galaxy::api::ChatRoomID chatRoomID) override;
			void OnChatRoomWithUserRetrieveFailure(galaxy::api::GalaxyID galaxyID, galaxy::api::IChatRoomWithUserRetrieveListener::FailureReason failureReason) override;
			void OnFriendListRetrieveSuccess() override;
			void OnFriendListRetrieveFailure(galaxy::api::IFriendListListener::FailureReason failureReason) override;

			std::string newChatMessage;
			std::string chattingHint{"Friend not selected"};
			networking::ChatRoomPtr chatRoom;
			std::vector<galaxy::api::GalaxyID> friends;
			std::vector<system::GUIElementPtr> guiElements;
		};

	}
}

#endif