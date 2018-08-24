#include "TailPart.h"

using namespace galaxy::demo;

TailPart::TailPart(const glm::vec3& _position, const glm::vec4& _color)
	: GameObject(_position)
	, color(_color)
{
}

const glm::vec4& TailPart::GetColor() const
{
	return color;
}

void TailPart::SetColor(const glm::vec4& _color)
{
	color = _color;
}