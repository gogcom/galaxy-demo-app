#include "NetworkingEngine.h"
#include <cassert>

using namespace gogtron::networking;

bool NetworkingEngine::SendMessage(const std::string& message, const galaxy::api::GalaxyID& userID)
{
	assert(userID.IsValid());
	return galaxy::api::Networking()->SendP2PPacket(userID, message.c_str(), message.length(), galaxy::api::P2P_SEND_RELIABLE);
}