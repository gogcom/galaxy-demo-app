#include "Sprite.h"

using namespace gogtron::renderer;

Sprite::Sprite(std::uint32_t _x, std::uint32_t _y, std::uint32_t _width, std::uint32_t _height)
	: x(_x)
	, y(_y)
	, width(_width)
	, height(_height)
{
}

std::uint32_t Sprite::GetX() const
{
    return x;
}

std::uint32_t Sprite::GetY() const
{
    return y;
}

std::uint32_t Sprite::GetWidth() const
{
    return width;
}

std::uint32_t Sprite::GetHeight() const
{
    return height;
}