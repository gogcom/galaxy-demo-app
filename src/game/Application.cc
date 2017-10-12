#include "Application.h"
#include <engine/renderer/OGLRenderer.h>
#include "Gogtron.h"

#if defined(_WIN32)
#include <shellapi.h>
#include <cstdio>
#endif

using namespace gogtron;

#if defined(_WIN32)
PCHAR* Application::Helper::CommandLineToArgvA(PCHAR CmdLine, int* _argc)
{
	PCHAR* argv;
	PCHAR  _argv;
	ULONG   len;
	ULONG   argc;
	CHAR   a;
	ULONG   i, j;

	BOOLEAN  in_QM;
	BOOLEAN  in_TEXT;
	BOOLEAN  in_SPACE;

	len = strlen(CmdLine);
	i = ((len + 2) / 2) * sizeof(PVOID) + sizeof(PVOID);

	argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
		i + (len + 2) * sizeof(CHAR));

	_argv = (PCHAR)(((PUCHAR)argv) + i);

	argc = 0;
	argv[argc] = _argv;
	in_QM = FALSE;
	in_TEXT = FALSE;
	in_SPACE = TRUE;
	i = 0;
	j = 0;

	while (a = CmdLine[i])
	{
		if (in_QM)
		{
			if (a == '\"')
			{
				in_QM = FALSE;
			}
			else
			{
				_argv[j] = a;
				j++;
			}
		}
		else
		{
			switch (a)
			{
				case '\"':
					in_QM = TRUE;
					in_TEXT = TRUE;
					if (in_SPACE)
					{
						argv[argc] = _argv + j;
						argc++;
					}
					in_SPACE = FALSE;
					break;
				case ' ':
				case '\t':
				case '\n':
				case '\r':
					if (in_TEXT)
					{
						_argv[j] = '\0';
						j++;
					}
					in_TEXT = FALSE;
					in_SPACE = TRUE;
					break;
				default:
					in_TEXT = TRUE;
					if (in_SPACE)
					{
						argv[argc] = _argv + j;
						argc++;
					}
					_argv[j] = a;
					j++;
					in_SPACE = FALSE;
					break;
			}
		}
		i++;
	}
	_argv[j] = '\0';
	argv[argc] = NULL;

	(*_argc) = argc;
	return argv;
}
#endif

Application::Application(int _argc, char** _argv)
	: argc(_argc)
	, argv(_argv)
	, window(std::make_shared<system::SDLWindow>("gogtron", 1280, 720))
	, renderEngine(std::make_shared<renderer::OGLRenderer>(window))
	, game(std::make_shared<GogTron>(renderEngine))
{
}

int Application::Run()
{
	if (!window->Init())
		return -1;

	if (!game->Init(argc, argv))
		return -2;

	while (game->IsRunning())
	{
		HandleWindowEvents();
		game->Update();
	}

	game->Release();

	window->Release();
	return 0;
}

void Application::HandleWindowEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			game->Close();
			break;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			game->OnMouseDown(event.button.x, event.button.y);
		}
		else if (event.type == SDL_MOUSEMOTION)
		{
			game->OnMouseMotion(event.motion.x, event.motion.y);
		}
		else if (event.type == SDL_KEYDOWN)
		{
			game->OnKeyDown(event.key.keysym);
		}
	}
}
