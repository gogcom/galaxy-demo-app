#ifndef GALAXY_DEMO_RENDERER_SPRITE_H
#define GALAXY_DEMO_RENDERER_SPRITE_H

#include <cstdint>

namespace galaxy::demo
{
	namespace renderer
	{

		class Sprite
		{
		public:

			Sprite(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height);

			std::uint32_t GetX() const;
			std::uint32_t GetY() const;
			std::uint32_t GetWidth() const;
			std::uint32_t GetHeight() const;

		private:

			std::uint32_t x;
			std::uint32_t y;
			std::uint32_t width;
			std::uint32_t height;
		};

	}
}

#endif