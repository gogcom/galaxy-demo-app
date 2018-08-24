#include "Lobby.h"
#include "LobbyEvent.h"
#include "Server.h"
#include "Client.h"
#include <game/IGame.h>
#include <algorithm>

using namespace galaxy::demo;
using namespace galaxy::demo::networking;

Lobby::Lobby(const IGamePtr& game)
	: ILobby(game)
{
}

bool Lobby::Init()
{
	return true;
}

bool Lobby::Release()
{
	lobbyID = 0;
	lobbyMembers.clear();
	return true;
}

std::string Lobby::GetConnectionString() const
{
	assert(lobbyID.ToUint64() != 0);
	std::string connectionString = "+connect_" + std::to_string(lobbyID.ToUint64());
	return connectionString;
}

bool Lobby::ConnectToLobbyByConnectionString(const std::string& connectionString)
{
	const std::string connectionStringPrefix = "+connect_";
	if (connectionString.find(connectionStringPrefix) == std::string::npos)
		return false;

	const auto lobbyIDStr = connectionString.substr(connectionStringPrefix.size());
	galaxy::api::GalaxyID id = std::stoull(lobbyIDStr);
	try
	{
		galaxy::api::Matchmaking()->JoinLobby(id);
	}
	catch (const galaxy::api::IError&)
	{
		return false;
	}

	return true;
}

void Lobby::OnLobbyCreated(const galaxy::api::GalaxyID& _lobbyID, galaxy::api::LobbyCreateResult result)
{
	if (result != galaxy::api::LOBBY_CREATE_RESULT_SUCCESS)
	{
		game->OnLobbyEvent(LobbyEvent::LOBBY_CREATE_FAILURE);
		return;
	}

	lobbyID = _lobbyID;
	game->OnLobbyEvent(LobbyEvent::LOBBY_CREATE_SUCCESS);
	galaxy::api::Friends()->SetRichPresence("connect", GetConnectionString().c_str());
}

void Lobby::AddLobbyMember(const galaxy::api::GalaxyID& galaxyID)
{
	if (std::find(std::begin(lobbyMembers), std::end(lobbyMembers), galaxyID) == std::end(lobbyMembers))
		lobbyMembers.push_back(galaxyID);
}

void Lobby::OnLobbyEntered(const galaxy::api::GalaxyID& _lobbyID, galaxy::api::LobbyEnterResult result)
{
	if (result != galaxy::api::LOBBY_ENTER_RESULT_SUCCESS)
	{
		game->OnLobbyEvent(LobbyEvent::LOBBY_JOIN_FAILURE);
		game->SetGameState(scene::GameState::State::START_MENU);
		return;
	}

	for (uint32_t i = 0; i < galaxy::api::Matchmaking()->GetNumLobbyMembers(_lobbyID); ++i)
	{
		AddLobbyMember(galaxy::api::Matchmaking()->GetLobbyMemberByIndex(_lobbyID, i));
	}

	lobbyID = _lobbyID;
	game->SetGameState(scene::GameState::State::IN_LOBBY_MENU);
	game->OnLobbyEvent(LobbyEvent::LOBBY_JOIN_SUCCESS);
	auto client = std::make_shared<networking::Client>(game);
	client->SetServerID(game->GetLobby()->GetLobbyID());
	game->SetClient(client);
	game->GetGameManager().SetClientState(GameManager::ClientState::INIT);
}

void Lobby::OnLobbyMemberStateChanged(const galaxy::api::GalaxyID& lobbyID, const galaxy::api::GalaxyID& memberID, galaxy::api::LobbyMemberStateChange memberStateChange)
{
	switch (memberStateChange)
	{
		case galaxy::api::LOBBY_MEMBER_STATE_CHANGED_ENTERED:
			AddLobbyMember(memberID);
			break;

		case galaxy::api::LOBBY_MEMBER_STATE_CHANGED_LEFT:
		case galaxy::api::LOBBY_MEMBER_STATE_CHANGED_DISCONNECTED:
		case galaxy::api::LOBBY_MEMBER_STATE_CHANGED_KICKED:
			lobbyMembers.erase(std::remove_if(std::begin(lobbyMembers), std::end(lobbyMembers), [&](const galaxy::api::GalaxyID& userID) { return userID == memberID; }));
			game->OnLobbyEvent(LobbyEvent::LOBBY_MEMBER_LEFT);
			break;

		default:
			break;
	}
}

void Lobby::OnPersonaDataChanged(galaxy::api::GalaxyID userID, uint32_t personaStateChange)
{
}

void Lobby::OnLobbyOwnerChanged(const galaxy::api::GalaxyID& lobbyID, const galaxy::api::GalaxyID& newOwnerID)
{
	if (newOwnerID == galaxy::api::User()->GetGalaxyID())
	{
		{
			auto server = game->GetServer();
			if (server)
				server.reset();
		}

		const auto& client = game->GetClient();
		if (!client)
			return;

		auto server = std::make_shared<networking::Server>(game);
		server->SetPlayers(game->GetGameManager().GetPlayers());
		game->SetServer(server);
		switch (game->GetGameManager().GetClientState())
		{
			case GameManager::ClientState::INIT:
				game->GetGameManager().SetServerState(GameManager::ServerState::INIT);
				break;

			case GameManager::ClientState::START_GAME:
				game->GetGameManager().SetServerState(GameManager::ServerState::INIT);
				break;

			case GameManager::ClientState::GAME:
				game->GetGameManager().SetServerState(GameManager::ServerState::GAME);
				break;

			case GameManager::ClientState::RESULT:
				game->GetGameManager().SetServerState(GameManager::ServerState::RESULTS);
				break;
		}
	}
}