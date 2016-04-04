#ifndef GOGTRON_NETWORKING_LOBBY_H
#define GOGTRON_NETWORKING_LOBBY_H

#include "ILobby.h"

namespace gogtron
{
	namespace networking
	{

		class Lobby : public ILobby

		{
		public:

			Lobby(const IGamePtr& game);

			virtual bool Init() override;
			virtual bool Release() override;

			virtual std::string GetConnectionString() const override;

			static bool ConnectToLobbyByConnectionString(const std::string& connectionString);

		private:

			virtual void OnLobbyCreated(const galaxy::api::GalaxyID& lobbyID, galaxy::api::LobbyCreateResult result) override;
			virtual void OnLobbyEntered(const galaxy::api::GalaxyID& lobbyID, galaxy::api::LobbyEnterResult result) override;
			virtual void OnLobbyMemberStateChanged(const galaxy::api::GalaxyID& lobbyID, const galaxy::api::GalaxyID& memberID, galaxy::api::LobbyMemberStateChange memberStateChange) override;
			virtual void OnPersonaDataChanged(galaxy::api::GalaxyID userID, uint32_t personaStateChange) override;
			virtual void OnLobbyOwnerChanged(const galaxy::api::GalaxyID& lobbyID, const galaxy::api::GalaxyID& newOwnerID) override;
		};

	}
}

#endif