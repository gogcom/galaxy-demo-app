#ifndef GALAXY_DEMO_NETWORKING_CHAT_ROOM_H
#define GALAXY_DEMO_NETWORKING_CHAT_ROOM_H

#include <galaxy/GalaxyApi.h>
#include <memory>
#include <string>
#include <list>

namespace galaxy::demo
{
	class IGame;
	using IGamePtr = std::shared_ptr<IGame>;

	namespace networking
	{

		class ChatRoom
			: public galaxy::api::IChatRoomMessageSendListener
			, public galaxy::api::IChatRoomMessagesRetrieveListener
			, public galaxy::api::GlobalChatRoomMessagesListener
		{
		public:

			explicit ChatRoom(galaxy::api::ChatRoomID chatRoomID, size_t maxChatMessages = 10);
			~ChatRoom() override;

			void SendChatMessage(const std::string& message);

			galaxy::api::ChatRoomID GetChatRoomID() const;
			const std::list<std::string>& GetChatMessages() const;

		private:

			void PopExcessMessages();
			void StoreNewMessage(const char* message);
			void StoreNewMessage(const galaxy::api::GalaxyID& sender, const std::string& buffer, size_t sz);

			void OnChatRoomMessageSendSuccess(galaxy::api::ChatRoomID chatRoomID, uint32_t sentMessageIndex, galaxy::api::ChatMessageID messageID, uint32_t sendTime) override;
			void OnChatRoomMessageSendFailure(galaxy::api::ChatRoomID chatRoomID, uint32_t sentMessageIndex, galaxy::api::IChatRoomMessageSendListener::FailureReason failureReason) override;
			void OnChatRoomMessagesRetrieveSuccess(galaxy::api::ChatRoomID chatRoomID, uint32_t messageCount, uint32_t longestMessageLength) override;
			void OnChatRoomMessagesRetrieveFailure(galaxy::api::ChatRoomID chatRoomID, galaxy::api::IChatRoomMessagesRetrieveListener::FailureReason failureReason) override;
			void OnChatRoomMessagesReceived(galaxy::api::ChatRoomID chatRoomID, uint32_t messageCount, uint32_t longestMessageLength) override;

			const galaxy::api::ChatRoomID chatRoomID;
			const size_t maxChatMessages;

			std::list<std::string> messages;

			// buffer variables
			galaxy::api::ChatMessageID messageID;
			galaxy::api::ChatMessageType messageType;
			galaxy::api::GalaxyID senderID;
			uint32_t sendTime;
			std::string buffer;
		};

		using ChatRoomPtr = std::unique_ptr<ChatRoom>;

	}
}

#endif