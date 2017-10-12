#include "GameObject.h"

using namespace gogtron;

std::uint32_t GameObject::GameObjectId = 0;

GameObject::GameObject(const glm::vec3& _position)
	: position(_position)
	, id(++GameObjectId)
{
}

const glm::vec3& GameObject::GetPosition() const
{
	return position;
}

std::uint32_t GameObject::GetId() const
{
	return id;
}

void GameObject::SetPosition(const glm::vec3& _position)
{
	position = _position;
}