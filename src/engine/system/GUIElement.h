#ifndef GOGTRON_SYSTEM_GUI_ELEMENT_H
#define GOGTRON_SYSTEM_GUI_ELEMENT_H

#include <engine/renderer/Renderer.h>
#include <engine/renderer/Sprite.h>
#include <string>
#include <functional>
#include <cstdint>
#include <memory>

namespace gogtron
{
	namespace system
	{

		using GUINotification = std::function<void()>;

		class GUIElement
		{
		public:

			GUIElement(const std::string& _textureName, const renderer::Sprite& _sprite, const GUINotification& _guiNotification)
				: textureName(_textureName)
				, sprite(_sprite)
				, guiNotification(_guiNotification)
			{
			}

			virtual void OnMouseDown(std::uint32_t x, std::uint32_t y) = 0;
			virtual void OnMouseMotion(std::uint32_t x, std::uint32_t y) = 0;

			virtual void Display(const renderer::RendererPtr& renderEngine) = 0;

			const renderer::Sprite& GetSprite() const
			{
				return sprite;
			}

		protected:

			const std::string textureName;
			GUINotification guiNotification;
			renderer::Sprite sprite;
		};

		using GUIElementPtr = std::shared_ptr<GUIElement>;

	}
}

#endif