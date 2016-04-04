#ifndef GOGTRON_PHYSICS_PHYSICS_ENGINE_H
#define GOGTRON_PHYSICS_PHYSICS_ENGINE_H

#include <engine/renderer/Sprite.h>
#include <cstdint>

namespace gogtron
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