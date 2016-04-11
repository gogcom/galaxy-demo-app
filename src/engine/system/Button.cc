#include "Button.h"
#include <engine/core/SDLResourceManager.h>
#include <engine/physics/PhysicsEngine.h>

using namespace gogtron;
using namespace gogtron::system;

Button::Button(const std::string& _textureName, const renderer::Sprite& _sprite, const GUINotification& _guiNotification)
	: GUIElement(_textureName, _sprite, _guiNotification)
	, currentDisplayedTextureName(textureName)
{
}

Button::Button(const std::string& _textureName, const std::string& _secondTextureName, const renderer::Sprite& _sprite, const GUINotification& _guiNotification)
	: GUIElement(_textureName, _sprite, _guiNotification)
	, currentDisplayedTextureName(textureName)
	, secondTextureName(_secondTextureName)
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
	const auto texture = core::SDLResourceManager::GetInstance().GetTexture(currentDisplayedTextureName);
	renderEngine->DisplayTexture(texture, sprite);
}