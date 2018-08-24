#ifndef GALAXY_DEMO_RENDERER_RENDERER_H
#define GALAXY_DEMO_RENDERER_RENDERER_H

#include "Color.h"
#include "Sprite.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <string>
#include <cstdint>

namespace galaxy::demo
{
	namespace renderer
	{

		class Renderer
		{
		public:

			virtual ~Renderer() = default;

			virtual bool StartScene() = 0;
			virtual bool EndScene() = 0;

			virtual bool DisplayText(const std::string& text, const Sprite& sprite, const std::string& fontName, const Color& color) = 0;
			virtual bool DisplayTexture(int texture, const Sprite& sprite) = 0;
			virtual bool Display3DGrid() = 0;
			virtual bool DisplayCube(const glm::vec3& position, const glm::vec4& color) = 0;
			virtual bool DisplayTrial(const glm::vec3& position, float angle, const glm::vec3& rotation, const glm::vec4& color) = 0;
		};

		using RendererPtr = std::shared_ptr<Renderer>;

	}
}

#endif