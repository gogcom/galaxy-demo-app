#ifndef GOGTRON_NETWORKING_LOBBY_EVENT_H
#define GOGTRON_NETWORKING_LOBBY_EVENT_H

namespace gogtron
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