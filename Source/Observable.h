#ifndef GAME_OBSERVABLE_H
#define GAME_OBSERVABLE_H

#include "EventType.h"
#include <vector>

namespace game
{

	class IObserver;

	class Observable
	{
	public:

		void AddObserver(IObserver* observer);
		void RemoveObserver(IObserver* observer);
		void Notify(const EventType& eventType);

	private:

		std::vector<IObserver*> observers;
	};

}

#endif