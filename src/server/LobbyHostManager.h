#pragma once

#include <galaxy/GalaxyGameServerApi.h>

#include "LobbyHost.h"

#include <memory>

#include <unordered_map>

namespace std
{

	template <>
	struct hash<galaxy::api::GalaxyID>
	{
		std::size_t operator()(const galaxy::api::GalaxyID& galaxyID) const
		{
			return static_cast<size_t>(galaxyID.ToUint64());
		}
	};

}

namespace galaxy::demo::server
{

	class LobbyHostManager
		: public galaxy::api::GameServerGlobalLobbyCreatedListener
		, public galaxy::api::GameServerGlobalLobbyEnteredListener
		, public galaxy::api::GameServerGlobalLobbyLeftListener
	{
	public:

		LobbyHostManager();

		void Tick();

	private:

		void SpawnLobbies();

		void SpawnLobby();

		void OnLobbyCreated(const galaxy::api::GalaxyID& lobbyID, galaxy::api::LobbyCreateResult result) override;

		void OnLobbyEntered(const galaxy::api::GalaxyID& lobbyID, galaxy::api::LobbyEnterResult result) override;

		void OnLobbyLeft(const galaxy::api::GalaxyID& lobbyID, galaxy::api::ILobbyLeftListener::LobbyLeaveReason leaveReason) override;

		void LobbyCreationFailed();

		void AddLobbyHost(const galaxy::api::GalaxyID& lobbyID);

		std::unordered_map<galaxy::api::GalaxyID, LobbyHost> lobbyList;
		uint32_t pendingLobbyCreateCount;
	};

}