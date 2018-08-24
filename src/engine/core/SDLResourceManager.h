#ifndef GALAXY_DEMO_CORE_SDLRESOURCE_MANAGER_H
#define GALAXY_DEMO_CORE_SDLRESOURCE_MANAGER_H

#include "ResourceManager.h"
#include <SDL_ttf.h>
#include <SDL_opengl.h>

namespace galaxy::demo
{
	namespace core
	{

		class SDLResourceManager : public ResourceManager<SDL_Surface, GLuint, TTF_Font>
		{
		public:

			virtual bool LoadFont(const std::string& path, const std::string& fontName) override;
			virtual bool LoadTexture(const std::string& path, const std::string& textureName) override;

			virtual bool CreateTextureFromFont(const FontPtr<TTF_Font>& font, const std::string& text, const renderer::Color& color, const std::string& textureName) override;

			static SDLResourceManager& GetInstance();
			static void ReleaseInstance();

		private:

			virtual bool LoadTexture(const TexturePtr<SDL_Surface>& texture, const std::string& textureName) override;

			static std::unique_ptr<SDLResourceManager> instance;
		};

	}
}

#endif