#include "DedicatedServer.h"

#include <iostream>

int main(int /*argc*/, char** /*argv*/)
{
	std::cout << "Starting a dedicated server..." << std::endl;

	galaxy::demo::server::DedicatedServer server;

	std::cout << "Ready to accept players" << std::endl;
	server.ServerLoop();
	std::cout << "Goodbye" << std::endl;
}
