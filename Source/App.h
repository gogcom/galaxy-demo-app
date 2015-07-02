#ifndef APP_H
#define APP_H

#include "Observable.h"

namespace game
{

	class Game;

}

class App : public game::Observable
{
public:

	App();

	~App();

	void Run();

private:

	void Clean();

private:

	game::Game* game;
};

#endif