#include "ChatRoom.h"
#include "Server.h"
#include "Client.h"
#include <game/IGame.h>

using namespace galaxy::demo;
using namespace galaxy::demo::networking;

namespace
{
	const char* GetPersonaName(const galaxy::api::GalaxyID& galaxyID)
	{
		const char* friendPersonaName = galaxy::api::Friends()->GetFriendPersonaName(galaxyID);
		return galaxy::api::GetError() ? "Unknown" : friendPersonaName;
	}
}

ChatRoom::ChatRoom(galaxy::api::ChatRoomID _chatRoomID, size_t _maxChatMessages)
	: chatRoomID(_chatRoomID)
	, maxChatMessages(_maxChatMessages)
{
}

ChatRoom::~ChatRoom()
{
	galaxy::api::ListenerRegistrar()->Unregister(IChatRoomMessageSendListener::GetListenerType(), static_cast<IChatRoomMessageSendListener*>(this));
	galaxy::api::ListenerRegistrar()->Unregister(IChatRoomMessagesRetrieveListener::GetListenerType(), static_cast<IChatRoomMessagesRetrieveListener*>(this));
}

void ChatRoom::SendChatMessage(const std::string& message)
{
	if (!chatRoomID)
		return;

	galaxy::api::Chat()->SendChatRoomMessage(chatRoomID, message.c_str(), this);
}

galaxy::api::ChatRoomID ChatRoom::GetChatRoomID() const
{
	return chatRoomID;
}

const std::list<std::string>& ChatRoom::GetChatMessages() const
{
	return messages;
}

void ChatRoom::PopExcessMessages()
{
	if (messages.size() >= maxChatMessages)
		messages.pop_front();
}

void ChatRoom::StoreNewMessage(const char* message)
{
	PopExcessMessages();
	messages.emplace_back(message);
}

void ChatRoom::StoreNewMessage(const galaxy::api::GalaxyID& sender, const std::string& buffer, size_t sz)
{
	PopExcessMessages();
	messages.emplace_back(GetPersonaName(sender));
	messages.back().append(": ");
	messages.back().append(buffer, 0, sz);
}

void ChatRoom::OnChatRoomMessageSendSuccess(galaxy::api::ChatRoomID chatRoomID, uint32_t sentMessageIndex, galaxy::api::ChatMessageID messageID, uint32_t sendTime)
{
}

void ChatRoom::OnChatRoomMessageSendFailure(galaxy::api::ChatRoomID chatRoomID, uint32_t sentMessageIndex, galaxy::api::IChatRoomMessageSendListener::FailureReason failureReason)
{
	switch(failureReason)
	{
	case IChatRoomMessageSendListener::FAILURE_REASON_UNDEFINED:
		StoreNewMessage("Sending message failed.");
		break;

	case IChatRoomMessageSendListener::FAILURE_REASON_FORBIDDEN:
		StoreNewMessage("Cannot send messages to this user.");
		break;

	case IChatRoomMessageSendListener::FAILURE_REASON_CONNECTION_FAILURE:
		StoreNewMessage("Cannot establish connection.");
		break;

	default: assert(false);
	}
}

void ChatRoom::OnChatRoomMessagesRetrieveSuccess(galaxy::api::ChatRoomID chatRoomID, uint32_t messageCount, uint32_t longestMessageLength)
{
	assert(messageCount <= maxChatMessages);
	this->OnChatRoomMessagesReceived(chatRoomID, messageCount, longestMessageLength);
}

void ChatRoom::OnChatRoomMessagesRetrieveFailure(galaxy::api::ChatRoomID chatRoomID, galaxy::api::IChatRoomMessagesRetrieveListener::FailureReason failureReason)
{
	switch(failureReason)
	{
	case IChatRoomMessagesRetrieveListener::FAILURE_REASON_UNDEFINED:
		StoreNewMessage("Retrieving past messages failed.");
		break;

	case IChatRoomMessagesRetrieveListener::FAILURE_REASON_FORBIDDEN:
		StoreNewMessage("Cannot retrieve messages from this user.");
		break;

	case IChatRoomMessagesRetrieveListener::FAILURE_REASON_CONNECTION_FAILURE:
		StoreNewMessage("Cannot establish connection.");
		break;

	default: assert(false);
	}
}

void ChatRoom::OnChatRoomMessagesReceived(galaxy::api::ChatRoomID _chatRoomID, uint32_t messageCount, uint32_t longestMessageLength)
{
	if(chatRoomID != _chatRoomID || !longestMessageLength)
		return;

	const auto chat = galaxy::api::Chat();

	if(buffer.size() < longestMessageLength)
		buffer.resize(longestMessageLength);

	for (uint32_t i = 0; i < messageCount; ++i)
	{
		auto sz = chat->GetChatRoomMessageByIndex(i, messageID, messageType, senderID, sendTime, &buffer.front(), longestMessageLength + 1);
		StoreNewMessage(senderID, buffer, sz);
	}
}
