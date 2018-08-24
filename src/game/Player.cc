#include "Player.h"

using namespace galaxy::demo;

galaxy::demo::Player::Player(const galaxy::api::GalaxyID& _galaxyID)
	: Player{_galaxyID, {}, {}, {}, {}, {}, false, 0}
{
}

Player::Player(
	const galaxy::api::GalaxyID& _galaxyID,
	const glm::vec3& _position,
	const glm::vec2& _direction,
	const glm::vec4& _color,
	const glm::vec2& _lastDirection,
	const glm::vec3& _lastTailPosition,
	bool _isAlive,
	std::uint32_t _points)
	: galaxyID(_galaxyID)
	, position(_position)
	, direction(_direction)
	, color(_color)
	, lastTailPosition(_lastTailPosition)
	, lastDirection(_lastDirection)
	, isAlive(_isAlive)
	, isReady(false)
	, points(_points)
{

}

bool Player::operator=(const PlayerPtr& other) const
{
	return galaxyID == other->galaxyID;
}

const galaxy::api::GalaxyID& Player::GetGalaxyID() const
{
	return galaxyID;
}

const glm::vec3& Player::GetPosition() const
{
	return position;
}

const glm::vec4& Player::GetColor() const
{
	return color;
}

const glm::vec2& Player::GetDirection() const
{
	return direction;
}

const glm::vec2& Player::GetLastDirection() const
{
	return lastDirection;
}

const glm::vec3& Player::GetLastTailPosition() const
{
	return lastTailPosition;
}

void Player::SetGalaxyID(const galaxy::api::GalaxyID& _galaxyID)
{
	galaxyID = _galaxyID;
}

void Player::SetPosition(const glm::vec3& _position)
{
	position = _position;
}

void Player::SetColor(const glm::vec4& _color)
{
	color = _color;
}

void Player::SetDirection(const glm::vec2& _direction)
{
	direction = _direction;
}

void Player::SetLastDirection(const glm::vec2& _direction)
{
	lastDirection = _direction;
}

void Player::SetLastTailPosition(const glm::vec3& _position)
{
	lastTailPosition = _position;
}

const std::vector<TailPartPtr> Player::GetTail() const
{
	return tail;
}

void Player::AddTailPart(const glm::vec3& tailPosition)
{
	auto tailPart = std::make_shared<TailPart>(tailPosition, glm::vec4(color.x, color.y, color.z, 0.4));
	tail.push_back(tailPart);
}

void Player::SetIsAlive(bool _isAlive)
{
	isAlive = _isAlive;
}

bool Player::IsAlive() const
{
	return isAlive;
}

void Player::SetIsReady(bool _isReady)
{
	isReady = _isReady;
}

bool Player::IsReady() const
{
	return isReady;
}

void Player::SetPoints(std::uint32_t _points)
{
	points = _points;
}

std::uint32_t Player::GetPoints() const
{
	return points;
}
