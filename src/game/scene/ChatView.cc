#include "ChatView.h"
#include <game/IGame.h>
#include <engine/system/Button.h>

namespace galaxy::demo::scene
{

	namespace
	{
		constexpr auto MAX_MESSAGE_LENGTH = 128;
		constexpr auto MESSAGE_OFFSET_Y = 50;
		constexpr size_t MAX_CHAT_MESSAGES = 10;
		constexpr auto TYPE_MESSAGE_OFFSET_Y = 100 + MESSAGE_OFFSET_Y * MAX_CHAT_MESSAGES;
	}

	ChatView::ChatView(const galaxy::demo::IGamePtr& game)
		: GameState(game)
	{
	}

	ChatView::~ChatView()
	{
		// prevent incoming callbacks of specific listener if object happens to die before their call
		galaxy::api::ListenerRegistrar()->Unregister(IChatRoomWithUserRetrieveListener::GetListenerType(), static_cast<IChatRoomWithUserRetrieveListener*>(this));
	}

	bool ChatView::Init()
	{
		const auto friends = galaxy::api::Friends();
		auto friendCount = friends->GetFriendCount();

		guiElements.reserve(friendCount + 1);

		for (uint32_t i = 0; i < friendCount; ++i)
		{
			auto userID = friends->GetFriendByIndex(i);

			guiElements.emplace_back(std::make_shared<system::Button>(
				friends->GetFriendPersonaName(userID), 100, 25 + 100 * i, 300, 100,
				[this, userID = std::move(userID)]()
				{
					galaxy::api::Chat()->RequestChatRoomWithUser(userID, this);
				}));
		}

		guiElements.emplace_back(std::make_shared<system::Button>(
			"BACK", 100, 600, 300, 100,
			[&]()
			{
				game->SetGameState(GameState::State::START_MENU);
			}));

		return true;
	}

	bool ChatView::Release()
	{
		return true;
	}

	void ChatView::OnMouseDown(std::uint32_t x, std::uint32_t y)
	{
		for (const auto& element : guiElements)
			element->OnMouseDown(x, y);
	}

	void ChatView::OnMouseMotion(std::uint32_t x, std::uint32_t y)
	{
		for (const auto& element : guiElements)
			element->OnMouseMotion(x, y);
	}

	void ChatView::OnKeyDown(SDL_Keysym key)
	{
		if (!chatRoom)
			return;

		if (key.sym == SDLK_BACKSPACE)
		{
			if(!newChatMessage.empty())
				newChatMessage.pop_back();
		}
		else if (key.sym == SDLK_RETURN)
		{
			chatRoom->SendChatMessage(newChatMessage);
			newChatMessage.clear();
		}
		else if (key.sym >= std::numeric_limits<char>::min()
			&& key.sym <= std::numeric_limits<char>::max()
			&& newChatMessage.size() < MAX_MESSAGE_LENGTH)
		{
			newChatMessage.push_back(key.sym);
		}
	}

	void ChatView::OnLobbyEvent(const networking::LobbyEvent& /*lobbyEvent*/)
	{
	}

	bool ChatView::Update()
	{
		return true;
	}

	bool ChatView::Display(const renderer::OGLRendererPtr& renderEngine)
	{
		for (const auto& element : guiElements)
			element->Display(renderEngine);

		renderEngine->DisplayText(chattingHint, renderer::Sprite(500, 50, chattingHint.length() * 10, 50), "FreeSans_" + chattingHint, SDL_Color{ 255, 0, 0, 255 });

		if (!chatRoom)
			return true;

		int lastY = 100;

		for (const auto& message : chatRoom->GetChatMessages())
		{
			renderEngine->DisplayText(message, renderer::Sprite(500, lastY, message.length() * 10, 50), message, SDL_Color{ 0, 255, 0, 255 });
			lastY += MESSAGE_OFFSET_Y;
		}

		if (newChatMessage.empty())
			renderEngine->DisplayText("Type message...", renderer::Sprite(500, TYPE_MESSAGE_OFFSET_Y, 150, 50), "FreeSans_TypeMessageHint", SDL_Color{ 0, 255, 0, 128 });
		else
			renderEngine->DisplayText(newChatMessage, renderer::Sprite(500, TYPE_MESSAGE_OFFSET_Y, newChatMessage.length() * 10, 50), "FreeSans_" + newChatMessage, SDL_Color{ 0, 255, 0, 255 });

		return true;
	}

	void ChatView::OnChatRoomWithUserRetrieveSuccess(galaxy::api::GalaxyID galaxyID, galaxy::api::ChatRoomID chatRoomID)
	{
		chattingHint = "Chatting with: ";

		const char* friendPersonaName = galaxy::api::Friends()->GetFriendPersonaName(galaxyID);
		chattingHint.append(galaxy::api::GetError() ? "Unknown" : friendPersonaName);

		chatRoom = std::make_unique<networking::ChatRoom>(chatRoomID, MAX_CHAT_MESSAGES);
		galaxy::api::Chat()->RequestChatRoomMessages(chatRoomID, MAX_CHAT_MESSAGES, 0, chatRoom.get());
	}

	void ChatView::OnChatRoomWithUserRetrieveFailure(galaxy::api::GalaxyID galaxyID, galaxy::api::IChatRoomWithUserRetrieveListener::FailureReason failureReason)
	{
		switch(failureReason)
		{
		case IChatRoomWithUserRetrieveListener::FAILURE_REASON_UNDEFINED:
			chattingHint = "Initiating chat failed.";
			break;

		case IChatRoomWithUserRetrieveListener::FAILURE_REASON_FORBIDDEN:
			chattingHint = "Communication with this user is not allowed.";
			break;

		case IChatRoomWithUserRetrieveListener::FAILURE_REASON_CONNECTION_FAILURE:
			chattingHint = "Unable to communicate with backend services.";
			break;

		default: assert(false);
		}
	}

	void ChatView::OnFriendListRetrieveSuccess()
	{
		// update friends list
		guiElements.clear();
		Init();
	}

	void ChatView::OnFriendListRetrieveFailure(galaxy::api::IFriendListListener::FailureReason failureReason)
	{
		chattingHint = "Unable to retrieve friends list.";
	}

}