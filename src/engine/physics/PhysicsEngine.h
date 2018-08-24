#ifndef GALAXY_DEMO_PHYSICS_PHYSICS_ENGINE_H
#define GALAXY_DEMO_PHYSICS_PHYSICS_ENGINE_H

#include <engine/renderer/Sprite.h>
#include <cstdint>

namespace galaxy::demo
{
	namespace physics
	{

		class PhysicsEngine
		{
		public:

			static bool IsPointInsideRect(std::uint32_t x, std::uint32_t y, const renderer::Sprite& sprite);
		};

	}
}

#endif