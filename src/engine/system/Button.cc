#include "Button.h"
#include <engine/core/SDLResourceManager.h>
#include <engine/physics/PhysicsEngine.h>

namespace galaxy::demo
{
	namespace system
	{

		namespace
		{

			constexpr uint32_t CAPTION_GLYPH_HIEGHT = 50;
			constexpr uint32_t CAPTION_GLYPH_WIDTH = 18;

		}

		Button::Button(const std::string& _caption, uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height, const GUINotification& _action)
			: GUIElement{"button", renderer::Sprite{_x, _y, _width, _height}, _action}
			, caption{_caption}
			, x{_x}
			, y{_y}
			, width{_width}
			, height{_height}
			, currentDisplayedTextureName{textureName}
			, secondTextureName{"selectedbutton"}
		{
		}

		void Button::OnMouseDown(std::uint32_t x, std::uint32_t y)
		{
			if (!physics::PhysicsEngine::IsPointInsideRect(x, y, sprite))
				return;

			guiNotification();
		}

		void Button::OnMouseMotion(std::uint32_t x, std::uint32_t y)
		{
			if (secondTextureName.empty())
				return;

			if (!physics::PhysicsEngine::IsPointInsideRect(x, y, sprite))
			{
				if (currentDisplayedTextureName == secondTextureName)
					currentDisplayedTextureName = textureName;

				return;
			}

			currentDisplayedTextureName = secondTextureName;
		}

		void Button::Display(const renderer::RendererPtr& renderEngine)
		{
			renderEngine->DisplayTexture(core::SDLResourceManager::GetInstance().GetTexture(currentDisplayedTextureName), sprite);
			renderEngine->DisplayText(
				caption,
				renderer::Sprite{
					static_cast<uint32_t>(x + (width - caption.length() * CAPTION_GLYPH_WIDTH) / 2),
					static_cast<uint32_t>(y + (height - CAPTION_GLYPH_HIEGHT) / 2),
					static_cast<uint32_t>(caption.length() * CAPTION_GLYPH_WIDTH),
					CAPTION_GLYPH_HIEGHT
				},
				"FreeSans_" + caption,
				SDL_Color{255, 0, 0, 255});
		}

	}
}