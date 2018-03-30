#include "DedicatedServer.h"

#include <galaxy/GalaxyExceptionHelper.h>

#include <chrono>
#include <thread>
#include <map>
#include <array>

namespace
{

	constexpr uint32_t MAX_PLAYERS = 4;
	constexpr auto TOPOLOGY_TYPE = galaxy::api::LOBBY_TOPOLOGY_TYPE_STAR;

	// TODO: can be increased once lobby id is sent with game messages so it's possible to distinguish lobbies
	constexpr uint32_t MAX_LOBBIES = 1;
}

namespace gogtron::server
{

	LobbyHostManager::LobbyHostManager()
		: pendingLobbyCreateCount{0}
	{
		SpawnLobbies();
	}

	void LobbyHostManager::Tick()
	{
		SpawnLobbies();

		for (auto& lobby : lobbyList)
			lobby.second.Tick();
	}

	void LobbyHostManager::SpawnLobbies()
	{
		while (pendingLobbyCreateCount + lobbyList.size() < MAX_LOBBIES)
			SpawnLobby();
	}

	void LobbyHostManager::SpawnLobby()
	{
		galaxy::api::GameServerLogger()->Info("Spawning new lobby: currentlobbyCount=%u", lobbyList.size());
		galaxy::api::GameServerMatchmaking()->CreateLobby(galaxy::api::LOBBY_TYPE_PUBLIC, MAX_PLAYERS, true, TOPOLOGY_TYPE);
		galaxy::api::ThrowIfGalaxyError();

		pendingLobbyCreateCount++;
	}

	void LobbyHostManager::OnLobbyCreated(const galaxy::api::GalaxyID& lobbyID, galaxy::api::LobbyCreateResult result)
	{
		assert(lobbyID.IsValid());
		assert(lobbyList.find(lobbyID) == std::end(lobbyList));
		assert(pendingLobbyCreateCount > 0);

		if (result != galaxy::api::LOBBY_CREATE_RESULT_SUCCESS)
		{
			galaxy::api::GameServerLogger()->Error("Failed to create a lobby: lobbyID=%llu", lobbyID.ToUint64());
			LobbyCreationFailed();
		}

		galaxy::api::GameServerLogger()->Info("Lobby create successfully: lobbyID=%llu", lobbyID.ToUint64());
	}

	void LobbyHostManager::OnLobbyEntered(const galaxy::api::GalaxyID& lobbyID, galaxy::api::LobbyEnterResult result)
	{
		assert(lobbyID.IsValid());
		assert(lobbyList.find(lobbyID) == std::end(lobbyList));
		assert(pendingLobbyCreateCount > 0);

		if (result != galaxy::api::LOBBY_ENTER_RESULT_SUCCESS)
		{
			static constexpr std::array<const char*, 4> failuresMap = {
				"The user has entered the lobby",
				"Specified lobby does not exist",
				"Specified lobby is full",
				"Unexpected error"};

			galaxy::api::GameServerLogger()->Error("Failed to enter a lobby: lobbyID=%llu, %s", lobbyID.ToUint64(), failuresMap[result]);
			LobbyCreationFailed();
		}

		pendingLobbyCreateCount--;
		assert(pendingLobbyCreateCount >= 0);

		AddLobbyHost(lobbyID);
	}

	void LobbyHostManager::LobbyCreationFailed()
	{
		pendingLobbyCreateCount--;
		assert(pendingLobbyCreateCount >= 0);

		galaxy::api::GameServerLogger()->Error("Failed to spawn a lobby: pendingLobbyCount=%u", pendingLobbyCreateCount);
	}

	void LobbyHostManager::AddLobbyHost(const galaxy::api::GalaxyID& lobbyID)
	{
		assert(lobbyID.IsValid());

		galaxy::api::GameServerLogger()->Info("Spawning lobby host: lobbyID = %llu, currentlobbyCount=%u", lobbyID.ToUint64(), lobbyList.size());
		lobbyList.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(lobbyID),
			std::forward_as_tuple(lobbyID));
	}

	void LobbyHostManager::OnLobbyLeft(const galaxy::api::GalaxyID& lobbyID, bool /*ioFailure*/)
	{
		assert(lobbyID.IsValid());
		assert(lobbyList.find(lobbyID) != std::end(lobbyList));

		lobbyList.erase(lobbyID);
		galaxy::api::GameServerLogger()->Info("Lobby left: lobbyID = %llu, lobbyCount=%u", lobbyID.ToUint64(), lobbyList.size());
	}

}
