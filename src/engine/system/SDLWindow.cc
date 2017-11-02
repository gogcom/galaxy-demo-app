#include "SDLWindow.h"
#include <SDL_ttf.h>

using namespace gogtron::system;

SDLWindow::SDLWindow(const std::string& _title, std::uint32_t _width, std::uint32_t _height)
	: Window(_title, _width, _height)
{
}

bool SDLWindow::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		return false;

	if (SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32) != 0
		|| SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16) != 0
		|| SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0)
	{
		return false;
	}

	handle = SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (!handle)
		return false;

	if (!InitOpenGL())
		return false;

	return true;
}

bool SDLWindow::Release()
{
	ReleaseOpenGL();

	SDL_DestroyWindow(handle);
	SDL_Quit();
	return true;
}

bool SDLWindow::InitOpenGL()
{
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	renderContext = SDL_GL_CreateContext(handle);
	const std::string errorMsg = SDL_GetError();
	if (!errorMsg.empty())
		return false;

	//SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
	//SDL_GL_SetSwapInterval(1);

	//Apparently this is necessary to build with Xcode
	glewExperimental = GL_TRUE;

	GLenum res = glewInit();
	if (res != GLEW_OK)
		return false;

	glViewport(0, 0, width, height);

	/*
	glEnable(GL_DEPTH_TEST);

	// We could make the view smaller inside
	// Our window if we wanted too.
	// The glViewport takes (x, y, width, height)
	// This basically means, what are our drawing boundaries

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	// The parameters are:
	// (view angle, aspect ration of the width to the height,
	//  the closest distance to the camera before it clips,
	// FOV		// Ratio				//  the farthest distance before it stops drawing).
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 150.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

	//glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	*/
	/*
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1280, 720, 1.0, -1.0, 1.0);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	*/

	if (TTF_Init())
		return false;

	return true;
}

bool SDLWindow::ReleaseOpenGL()
{
	TTF_Quit();
	SDL_GL_DeleteContext(renderContext);
	return true;
}