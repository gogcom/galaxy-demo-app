#include "AbstractServer.h"
#include "NetworkingEngine.h"
#include <algorithm>

using namespace galaxy::demo::networking;

AbstractServer::~AbstractServer() = default;

bool AbstractServer::SendMessageToAll(const std::string& message) const
{
	return std::all_of(std::begin(lobbyMembers), std::end(lobbyMembers), [&](const galaxy::api::GalaxyID& member) { return NetworkingEngine::SendMessage(message, member); });
}

void AbstractServer::SetLobbyMembers(const std::vector<galaxy::api::GalaxyID>& members)
{
	lobbyMembers = members;
}

void AbstractServer::AddLobbyMember(const galaxy::api::GalaxyID& member)
{
	lobbyMembers.push_back(member);
}

const std::vector<galaxy::api::GalaxyID>& AbstractServer::GetLobbyMembers() const
{
	return lobbyMembers;
}