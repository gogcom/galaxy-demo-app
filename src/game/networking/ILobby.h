#ifndef GALAXY_DEMO_NETWORKING_I_LOBBY_H
#define GALAXY_DEMO_NETWORKING_I_LOBBY_H

#include <galaxy/GalaxyApi.h>
#include <memory>
#include <vector>
#include <string>

namespace galaxy::demo
{

	class IGame;
	using IGamePtr = std::shared_ptr<IGame>;

	namespace networking
	{

		class ILobby
			: public galaxy::api::GlobalLobbyCreatedListener
			, public galaxy::api::GlobalLobbyEnteredListener
			, public galaxy::api::GlobalLobbyMemberStateListener
			, public galaxy::api::GlobalPersonaDataChangedListener
			, public galaxy::api::GlobalLobbyOwnerChangeListener
		{
		public:

			ILobby(const IGamePtr& _game)
				: game(_game)
			{
			}

			virtual ~ILobby()
			{
				galaxy::api::Matchmaking()->LeaveLobby(lobbyID);
			}

			virtual bool Init() = 0;
			virtual bool Release() = 0;

			virtual std::string GetConnectionString() const = 0;

			const galaxy::api::GalaxyID& GetLobbyID() const
			{
				return lobbyID;
			}

			const std::vector<galaxy::api::GalaxyID>& GetLobbyMembers() const
			{
				return lobbyMembers;
			}

		protected:

			virtual void OnLobbyCreated(const galaxy::api::GalaxyID& lobbyID, galaxy::api::LobbyCreateResult result) = 0;
			virtual void OnLobbyEntered(const galaxy::api::GalaxyID& lobbyID, galaxy::api::LobbyEnterResult result) = 0;
			virtual void OnLobbyMemberStateChanged(const galaxy::api::GalaxyID& lobbyID, const galaxy::api::GalaxyID& memberID, galaxy::api::LobbyMemberStateChange memberStateChange) = 0;
			virtual void OnPersonaDataChanged(galaxy::api::GalaxyID userID, uint32_t personaStateChange) = 0;
			virtual void OnLobbyOwnerChanged(const galaxy::api::GalaxyID& lobbyID, const galaxy::api::GalaxyID& newOwnerID) = 0;

			IGamePtr game;
			galaxy::api::GalaxyID lobbyID;
			std::vector<galaxy::api::GalaxyID> lobbyMembers;
		};

		using ILobbyPtr = std::shared_ptr<ILobby>;

	}
}

#endif