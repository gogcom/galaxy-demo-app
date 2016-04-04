#ifndef GOGTRON_SYSTEM_BUTTON_H
#define GOGTRON_SYSTEM_BUTTON_H

#include "GUIElement.h"

namespace gogtron
{
	namespace system
	{

		class Button : public GUIElement
		{
		public:

			Button(const std::string& _textureName, const renderer::Sprite& _sprite, const GUINotification& _guiNotification);
			Button(const std::string& _textureName, const std::string& _secondTextureName, const renderer::Sprite& _sprite, const GUINotification& _guiNotification);

			virtual void OnMouseDown(std::uint32_t x, std::uint32_t y) override;
			virtual void OnMouseMotion(std::uint32_t x, std::uint32_t y) override;
			virtual void Display(const renderer::RendererPtr& renderEngine) override;

		private:

			std::string currentDisplayedTextureName;
			const std::string secondTextureName;
		};

	}
}

#endif