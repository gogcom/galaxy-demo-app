#include "SDLResourceManager.h"
#include <SDL_image.h>

using namespace galaxy::demo;
using namespace galaxy::demo::core;

std::unique_ptr<SDLResourceManager> SDLResourceManager::instance = nullptr;

SDLResourceManager& SDLResourceManager::GetInstance()
{
	if (!instance)
		instance = std::make_unique<SDLResourceManager>();
	return *instance;
}

void SDLResourceManager::ReleaseInstance()
{
	instance.reset();
}

bool SDLResourceManager::LoadFont(const std::string& path, const std::string& fontName)
{
	if (fonts.find(fontName) != std::end(fonts))
		return true;

	FontPtr<TTF_Font> font(TTF_OpenFont(path.c_str(), 32), [](TTF_Font* f) { if (f) TTF_CloseFont(f); });

	if (!font)
		return false;

	fonts[fontName] = font;
	return true;
}

bool SDLResourceManager::LoadTexture(const std::string& path, const std::string& textureName)
{
	if (textures.find(textureName) != std::end(textures))
		return true;

	std::shared_ptr<SDL_Surface> texture(IMG_Load(path.c_str()), [](SDL_Surface* surface) { if (surface) SDL_FreeSurface(surface); });

	if (!texture)
		return false;

	GLuint object = 0;
	glGenTextures(1, &object);
	glBindTexture(GL_TEXTURE_2D, object);

	int mode = GL_RGB;
	if (texture->format->BytesPerPixel == 4)
		mode = GL_RGBA;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, mode, texture->w, texture->h, 0, mode, GL_UNSIGNED_BYTE, texture->pixels);

	textures[textureName] = object;
	return true;
}

bool SDLResourceManager::LoadTexture(const TexturePtr<SDL_Surface>& texture, const std::string& textureName)
{
	if (!texture)
		return false;

	GLuint object = 0;
	glGenTextures(1, &object);
	glBindTexture(GL_TEXTURE_2D, object);

	int mode = GL_RGB;
	if (texture->format->BytesPerPixel == 4)
		mode = GL_RGBA;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, mode, texture->w, texture->h, 0, mode, GL_UNSIGNED_BYTE, texture->pixels);

	textures[textureName] = object;
	return true;
}

bool SDLResourceManager::CreateTextureFromFont(const FontPtr<TTF_Font>& font, const std::string& text, const renderer::Color& color, const std::string& textureName)
{
	std::shared_ptr<SDL_Surface> textSurface(TTF_RenderText_Blended(font.get(), text.c_str(), color), [](SDL_Surface* s) { if (s) SDL_FreeSurface(s); });
	return LoadTexture(textSurface, textureName);
}