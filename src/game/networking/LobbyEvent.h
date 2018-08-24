#ifndef GALAXY_DEMO_NETWORKING_LOBBY_EVENT_H
#define GALAXY_DEMO_NETWORKING_LOBBY_EVENT_H

namespace galaxy::demo
{
	namespace networking
	{

		enum class LobbyEvent
		{
			LOBBY_CREATE_SUCCESS,
			LOBBY_CREATE_FAILURE,
			LOBBY_JOIN_SUCCESS,
			LOBBY_JOIN_FAILURE,
			LOBBY_MEMBER_LEFT
		};

	}
}

#endif