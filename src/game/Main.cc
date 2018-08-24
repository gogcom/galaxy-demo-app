#include "Application.h"

#if defined(_WIN32)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int argc;
	LPSTR* argv = galaxy::demo::Application::Helper::CommandLineToArgvA(lpCmdLine, &argc);
	galaxy::demo::Application app(argc, argv);
	return app.Run();
}

#else

int main(int argc, char** argv)
{
	galaxy::demo::Application app(argc, argv);
	return app.Run();
}

#endif