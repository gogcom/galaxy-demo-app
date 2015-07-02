#ifndef GAME_I_OBSERVER_H
#define GAME_I_OBSERVER_H

#include "EventType.h"

namespace game
{

	class IObserver
	{
	public:

		virtual ~IObserver() {}
		virtual void OnEvent(const EventType& eventType) = 0;
	};

}

#endif