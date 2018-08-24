#pragma once

#include <game/Player.h>
#include <engine/gamelogic/GameLogic.h>
#include <engine/utils/NonCopyable.h>

#include <galaxy/GalaxyGameServerApi.h>

#include <map>
#include <vector>

namespace galaxy::demo::server
{

	class LobbyHost
		: public galaxy::api::GameServerGlobalServerNetworkingListener
		, public galaxy::api::GameServerGlobalLobbyMemberStateListener
		, public utils::NonCopyable<LobbyHost>
	{
	public:

		~LobbyHost() = default;

		LobbyHost(const galaxy::api::GalaxyID& lobbyID);

		enum State
		{
			WAITING_FOR_PLAYERS,
			IN_GAME,
			FINISHED
		};

		void Tick();

	private:

		virtual void OnServerP2PPacketAvailable(uint32_t msgSize, uint8_t channel) override;
		virtual void OnLobbyMemberStateChanged(const galaxy::api::GalaxyID& _lobbyID, const galaxy::api::GalaxyID& memberID, galaxy::api::LobbyMemberStateChange memberStateChange) override;
		void OnMemberReady(const galaxy::api::GalaxyID& memberID);

		LobbyHost::State GetHostState() const;
		void SetHostState(State _state);

		bool ReadyToStart() const;
		std::string CreateInitGameMessage() const;
		bool StartGame();

		void SendGameTick();
		void FinishGame();
		void SendGameResults();

		void LeaveLobby() const;

		void BroadcastMessage(const std::string& message) const;

		std::vector<PlayerPtr> GetPlayers() const;

		std::map<const galaxy::api::GalaxyID, PlayerPtr> membersMap;

		std::unique_ptr<gamelogic::GameLogic> gameLogic;

		const galaxy::api::GalaxyID lobbyID;

		State state;
		uint64_t startGameTime;

	};

}
