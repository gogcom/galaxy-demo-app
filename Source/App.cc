#include "App.h"
#include "Game.h"
#include <cstdlib>

App::App()
	: game(new game::Game())
{
	AddObserver(game);
}

App::~App()
{
	Clean();
}

void App::Run()
{
	game->Init();
	game->Run();
	game->Clean();
}

void App::Clean()
{
	if (game)
	{
		delete game;
		game = NULL;
	}
}