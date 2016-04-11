#include "Color.h"

using namespace gogtron::renderer;

Color::Color(std::uint8_t _r, std::uint8_t _g, std::uint8_t _b, std::uint8_t _a)
	: r(_r)
	, g(_g)
	, b(_b)
	, a(_a)
{
}

void Color::SetR(std::uint8_t _r)
{
	r = _r;
}

void Color::SetG(std::uint8_t _g)
{
	g = _g;
}

void Color::SetB(std::uint8_t _b)
{
	b = _b;
}

void Color::SetA(std::uint8_t _a)
{
	a = _a;
}

std::uint8_t Color::GetR() const
{
	return r;
}

std::uint8_t Color::GetG() const
{
	return g;
}

std::uint8_t Color::GetB() const
{
	return b;
}

std::uint8_t Color::GetA() const
{
	return a;
}