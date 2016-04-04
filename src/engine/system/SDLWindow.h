#ifndef GOGTRON_ENGINE_SYSTEM_SDLWINDOW_H
#define GOGTRON_ENGINE_SYSTEM_SDLWINDOW_H

#include "Window.h"
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

namespace gogtron
{
	namespace system
	{

		class SDLWindow : public Window<SDL_Window*, SDL_GLContext>
		{
		public:

			SDLWindow(const std::string& title, std::uint32_t width, std::uint32_t height);

			virtual bool Init() override;
			virtual bool Release() override;

		private:

			bool InitOpenGL();
			bool ReleaseOpenGL();
		};

		using SDLWindowPtr = std::shared_ptr<SDLWindow>;

	}
}

#endif