#include "DedicatedServer.h"

#include <galaxy/GalaxyExceptionHelper.h>

#include <array>
#include <chrono>
#include <thread>
#include <map>

namespace
{
	// TODO: move to a common place
	constexpr auto CLIENT_ID = "48500786244777727";
	constexpr auto CLIENT_SECRET = "8573705eedaf4808fb704aeecc3763248849238b316e42f9e276b0bf40d01efe";

	constexpr auto SERVER_LOOP_TIMEOUT = std::chrono::milliseconds(10);
}

namespace gogtron::server
{

	DedicatedServer::DedicatedServer()
		: isServerDone{false}
	{
		galaxy::api::InitGameServer({CLIENT_ID, CLIENT_SECRET});
		galaxy::api::ThrowIfGalaxyError();

		authListener = std::make_unique<AuthListener>(*this);
		galaxy::api::GameServerUser()->SignInAnonymous();
		galaxy::api::ThrowIfGalaxyError();
	}

	DedicatedServer::~DedicatedServer()
	{
		galaxy::api::GameServerLogger()->Info("Shutting down the server...");

		lobbyHostManager.reset();

		// Unregister listeners before shutting the server down
		authListener.reset();

		galaxy::api::ShutdownGameServer();
	}

	bool DedicatedServer::IsServerDone()
	{
		return isServerDone;
	}

	void DedicatedServer::StopServer()
	{
		isServerDone = true;
	}

	void DedicatedServer::ServerLoop()
	{
		while (!IsServerDone())
		{
			std::this_thread::sleep_for(SERVER_LOOP_TIMEOUT);

			galaxy::api::ProcessGameServerData();
			galaxy::api::ThrowIfGalaxyError();

			if (lobbyHostManager)
				lobbyHostManager->Tick();
		}
	}

	void DedicatedServer::SpawnLobbyHostManager()
	{
		assert(lobbyHostManager.get() == nullptr);

		galaxy::api::GameServerLogger()->Info("Spawning lobby host manager");
		lobbyHostManager = std::make_unique<LobbyHostManager>();
	}

	DedicatedServer::AuthListener::AuthListener(DedicatedServer& _server)
		: server(_server)
	{
	}

	void DedicatedServer::AuthListener::OnAuthSuccess()
	{
		galaxy::api::GameServerLogger()->Info("Server authenticated successfully");
		server.SpawnLobbyHostManager();
	}

	void DedicatedServer::AuthListener::OnAuthFailure(FailureReason reason)
	{
		static constexpr auto failuresMap = std::array<const char*, 8> {
			"Undefined error",
			"Galaxy Service is not installed properly or fails to start",
			"Galaxy Service is not signed in properly",
			"Unable to communicate with backend services",
			"User that is being signed in has no license for this application",
			"Unable to match client credentials (ID, secret) or user credentials (username, password)",
			"Galaxy has not been initialized",
			"Unable to communicate with external service"};

		galaxy::api::GameServerLogger()->Error("Authentication failure: %s. Exiting", failuresMap[reason]);
		server.StopServer();
	}

	void DedicatedServer::AuthListener::OnAuthLost()
	{
		galaxy::api::GameServerLogger()->Error("Authentication lost. Exiting");
		server.StopServer();
	}
}
