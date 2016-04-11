#ifndef GOGTRON_CORE_RESOURCE_MANAGER_H
#define GOGTRON_CORE_RESOURCE_MANAGER_H

#include <engine/renderer/Color.h>
#include <string>
#include <unordered_map>
#include <memory>
#include <cassert>

namespace gogtron
{
	namespace core
	{

		template<typename TFont>
		using FontPtr = std::shared_ptr<TFont>;

		template<typename TTexture>
		using TexturePtr = std::shared_ptr<TTexture>;

		template<typename TTexture, typename TTextureID, typename TFont>
		class ResourceManager
		{
		public:

			virtual ~ResourceManager() = default;

			virtual bool LoadFont(const std::string& path, const std::string& fontName) = 0;
			virtual bool LoadTexture(const std::string& path, const std::string& textureName) = 0;

			virtual bool CreateTextureFromFont(const FontPtr<TFont>& font, const std::string& text, const renderer::Color& color, const std::string& textureName) = 0;

			const FontPtr<TFont>& GetFont(const std::string& fontName) const
			{
				const auto& font = fonts.find(fontName);
				assert(font != std::end(fonts));
				return font->second;
			}

			TTextureID GetTexture(const std::string& textureName) const
			{
				const auto& texture = textures.find(textureName);
				return texture != std::end(textures) ? texture->second : 0;
			}

		protected:

			virtual bool LoadTexture(const TexturePtr<TTexture>& texture, const std::string& textureName) = 0;

			std::unordered_map<std::string, FontPtr<TFont>> fonts;
			std::unordered_map<std::string, TTextureID> textures;
		};

	}
}

#endif