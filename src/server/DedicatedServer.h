#pragma once

#include "LobbyHostManager.h"

#include <galaxy/GalaxyGameServerApi.h>

#include <memory>

namespace galaxy::demo::server
{

	class DedicatedServer
	{
	public:

		DedicatedServer();
		~DedicatedServer();

		void ServerLoop();

		void StopServer();

		void SpawnLobbyHostManager();

	private:

		bool IsServerDone();

		class AuthListener : public galaxy::api::GameServerGlobalAuthListener
		{
		public:

			AuthListener(DedicatedServer& server);

			virtual void OnAuthSuccess() override;
			virtual void OnAuthFailure(FailureReason reason) override;
			virtual void OnAuthLost() override;

		private:

			DedicatedServer& server;
		};

		std::unique_ptr<AuthListener> authListener;

		std::unique_ptr<LobbyHostManager> lobbyHostManager;

		bool isServerDone;
	};

}
