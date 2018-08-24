#include "AbstractClient.h"
#include "NetworkingEngine.h"
#include <cassert>

using namespace galaxy::demo::networking;

AbstractClient::~AbstractClient() = default;

bool AbstractClient::SendMessage(const std::string& message, const galaxy::api::GalaxyID& userID)
{
	return NetworkingEngine::SendMessage(message, userID);
}

bool AbstractClient::SendMessageToServer(const std::string& message)
{
	return NetworkingEngine::SendMessage(message, serverID);
}

const galaxy::api::GalaxyID& AbstractClient::GetServerID() const
{
	return serverID;
}

void AbstractClient::SetServerID(const galaxy::api::GalaxyID& _serverID)
{
	serverID = _serverID;
}
