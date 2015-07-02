#ifndef GAME_H
#define GAME_H

#include "IObserver.h"
#include "Listeners.h"
#include "Input.h"
#include "GameContext.h"

namespace game
{

	class Game : public IObserver
	{
	public:

		virtual ~Game();

		virtual void OnEvent(const EventType& eventType);

		virtual void Init();

		virtual void Run();

		virtual void Clean();

	private:

		ListenerManager listenerManager;
		InputHandler inputHandler;
		GameContext gameContext;
	};

}

#endif