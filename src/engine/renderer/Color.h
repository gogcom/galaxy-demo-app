#ifndef GOGTRON_RENDERER_COLOR_H
#define GOGTRON_RENDERER_COLOR_H

#include <SDL.h>
#include <cstdint>

namespace gogtron
{
	namespace renderer
	{

		class Color
		{
		public:

			template<typename T>
			Color(const T& color)
				: r(color.r)
				, g(color.g)
				, b(color.b)
				, a(color.a)
			{
			}

			Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);

			void SetR(std::uint8_t _r);
			void SetG(std::uint8_t _g);
			void SetB(std::uint8_t _b);
			void SetA(std::uint8_t _a);

			std::uint8_t GetR() const;
			std::uint8_t GetG() const;
			std::uint8_t GetB() const;
			std::uint8_t GetA() const;

			template<typename T>
			operator T() const
			{
				return T{r, g, b, a};
			}

		private:

			std::uint8_t r;
			std::uint8_t g;
			std::uint8_t b;
			std::uint8_t a;
		};

	}
}

#endif