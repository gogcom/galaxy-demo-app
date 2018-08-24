#ifndef GALAXY_DEMO_RENDERER_OGLRENDERER_H
#define GALAXY_DEMO_RENDERER_OGLRENDERER_H

#include "AbstractRenderer.h"
#include <engine/system/SDLWindow.h>

namespace galaxy::demo
{
	namespace renderer
	{

		class OGLRenderer : public AbstractRenderer<system::SDLWindow>
		{
		public:

			OGLRenderer(const system::SDLWindowPtr& window);

			virtual bool StartScene() override;
			virtual bool EndScene() override;

			virtual bool DisplayText(const std::string& text, const Sprite& sprite, const std::string& fontName, const Color& color) override;
			virtual bool DisplayTexture(int texture, const Sprite& sprite) override;
			virtual bool Display3DGrid() override;
			virtual bool DisplayCube(const glm::vec3& position, const glm::vec4& color) override;
			virtual bool DisplayTrial(const glm::vec3& position, float angle, const glm::vec3& rotation, const glm::vec4& color) override;
		};

		using OGLRendererPtr = std::shared_ptr<OGLRenderer>;
	}
}

#endif