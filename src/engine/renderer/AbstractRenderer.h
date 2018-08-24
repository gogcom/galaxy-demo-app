#ifndef GALAXY_DEMO_RENDERER_ABSTRACT_RENDERER_H
#define GALAXY_DEMO_RENDERER_ABSTRACT_RENDERER_H

#include "Renderer.h"
#include <engine/system/Window.h>

namespace galaxy::demo
{
	namespace renderer
	{

		template<typename TWindow>
		class AbstractRenderer : public Renderer
		{
		public:

			AbstractRenderer(const std::shared_ptr<TWindow>& _window)
				: window(_window)
			{
			}

		protected:

			std::shared_ptr<TWindow> window;
		};

	}
}

#endif