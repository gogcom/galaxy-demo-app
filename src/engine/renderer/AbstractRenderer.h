#ifndef GOGTRON_RENDERER_ABSTRACT_RENDERER_H
#define GOGTRON_RENDERER_ABSTRACT_RENDERER_H

#include "Renderer.h"
#include <engine/system/Window.h>

namespace gogtron
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