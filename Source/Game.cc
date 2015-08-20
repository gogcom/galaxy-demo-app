#include "Game.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#if defined (_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace game
{

	static const char* CLIENT_ID = "46896872123840687";
	static const char* CLIENT_SECRET = "964c349c91348b913d49e4c1b48c7e65215c6f6f295dcced21f956730d1a4532";

	Game::~Game()
	{
	}

	void Game::OnEvent(const EventType& eventType)
	{
		switch (eventType)
		{
		case game::CLOSE_EVENT:
			gameContext.SetGameState(GameContext::QUIT);
			break;
		}
	}

	void Game::Init()
	{
		try
		{
			// initialize the GOG library
			galaxy::api::Init(CLIENT_ID, CLIENT_SECRET);
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Failed to initialize Galaxy, error=%s\n", error.GetMsg());

			// the game should support it and continue working without Galaxy.
			gameContext.SetGameState(GameContext::QUIT);
			return;
		}

		// create global listeners (all listeners require that the library is initialized)
		listenerManager.CreateListeners(gameContext);

		// sign in to the GOG Galaxy universe
		try
		{
			fprintf(stdout, "Signing in...\n");
			galaxy::api::User()->SignIn();
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Failed to sing in to Galaxy, error=%s\n", error.GetMsg());

			// the game should support it and continue working without Galaxy.
			gameContext.SetGameState(GameContext::QUIT);
			return;
		}

		// the authorization process will continue and the auth listener will be called.

		std::srand(static_cast<unsigned int>(std::time(0)));
	}

	void Game::Clean()
	{
		// delete global listeners (all listeners need to be deleted before the library shuts down)
		listenerManager.DeleteListeners();

		// trigger shutdown of the library
		try
		{
			galaxy::api::Shutdown();
		}
		catch (const galaxy::api::IError& error)
		{
			fprintf(stderr, "Failed to shout down Galaxy, error=%s\n", error.GetMsg());
		}
	}

	void Game::Run()
	{
		std::list<ICommand*> commands;

		// the main loop of the application
		while (gameContext != GameContext::QUIT)
		{
			// process Galaxy input/output operations
			galaxy::api::ProcessData();

			inputHandler.ReadCommands(commands);
			if (!commands.empty())
			{
				ICommand* command = commands.front();
				commands.pop_front();
				command->Execute(gameContext);
			}

			// delay simulating other works
#if defined (_WIN32)
			Sleep(1);
#else
			usleep(1000);
#endif
		}

		fprintf(stdout, "Press any key to quit\n");
		inputHandler.ReadKey();
	}

}
