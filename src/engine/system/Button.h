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

			Button(
				const std::string& caption,
				uint32_t x,
				uint32_t y,
				uint32_t width,
				uint32_t height,
				const GUINotification& _action);

			virtual void OnMouseDown(std::uint32_t x, std::uint32_t y) override;
			virtual void OnMouseMotion(std::uint32_t x, std::uint32_t y) override;
			virtual void Display(const renderer::RendererPtr& renderEngine) override;

		private:

			const std::string caption;
			const uint32_t x, y, width, height;
			std::string currentDisplayedTextureName;
			const std::string secondTextureName;
		};

	}
}

#endif