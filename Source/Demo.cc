#include "App.h"
#include "EventType.h"
#if defined (_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <signal.h>
#include <unistd.h>
#endif

App* app = NULL;

#if defined (_WIN32)
BOOL WINAPI CloseHandler(DWORD controlType)
{
	switch (controlType)
	{
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
		app->Notify(game::CLOSE_EVENT);
		return TRUE;

	default:
		return FALSE;
	}
}
#else
void CloseHandler(int signal)
{
	app->Notify(game::CLOSE_EVENT);
}
#endif

int main()
{
#if defined (_WIN32)
	SetConsoleCtrlHandler(CloseHandler, TRUE);
#else
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = CloseHandler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);
#endif

	app = new App();
	app->Run();
	delete app;
	app = NULL;

	return 0;
}