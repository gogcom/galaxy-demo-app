#include "Observable.h"
#include "IObserver.h"
#include <algorithm>

namespace game
{

	void Observable::AddObserver(IObserver* observer)
	{
		observers.push_back(observer);
	}

	void Observable::RemoveObserver(IObserver* observer)
	{
		observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
	}

	void Observable::Notify(const EventType& eventType)
	{
		for (std::vector<IObserver*>::iterator it = observers.begin(); it != observers.end(); ++it)
			(*it)->OnEvent(eventType);
	}

}