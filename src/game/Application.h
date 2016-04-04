#ifndef GOGTRON_APPLICATION_H
#define GOGTRON_APPLICATION_H

#include "IGame.h"
#include <engine/renderer/OGLRenderer.h>
#include <engine/system/SDLWindow.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEN
#include <windows.h>
#endif


namespace gogtron
{

	class Application
	{
	public:

#if defined(_WIN32)
		struct Helper
		{
			static PCHAR* CommandLineToArgvA(PCHAR CmdLine, int* argc);
		};
#endif

		Application(int argc, char** argv);
		int Run();

	private:

		void HandleWindowEvents();

		int argc;
		char** argv;
		system::SDLWindowPtr window;
		renderer::OGLRendererPtr renderEngine;
		IGamePtr game;
	};

}

#endif