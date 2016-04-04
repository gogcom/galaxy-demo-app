#include "Application.h"

#if defined(_WIN32)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int argc;
	LPSTR* argv = gogtron::Application::Helper::CommandLineToArgvA(lpCmdLine, &argc);
	gogtron::Application app(argc, argv);
	return app.Run();
}

#else

int main(int argc, char** argv)
{
	gogtron::Application app(argc, argv);
	return app.Run();
}

#endif