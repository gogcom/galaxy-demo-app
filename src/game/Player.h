#ifndef GOGTRON_PLAYER_H
#define GOGTRON_PLAYER_H

#include "TailPart.h"
#include <galaxy/GalaxyApi.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace gogtron
{

	class Player
	{
	public:

		Player() = default;

		Player(const galaxy::api::GalaxyID& galaxyID);

		Player(
			const galaxy::api::GalaxyID& galaxyID,
			const glm::vec3& position,
			const glm::vec2& direction,
			const glm::vec4& color,
			const glm::vec2& lastDirection,
			const glm::vec3& lastTailPosition,
			bool isAlive,
			std::uint32_t points);

		bool operator=(const std::shared_ptr<Player>& other) const;

		const galaxy::api::GalaxyID& GetGalaxyID() const;
		const glm::vec3& GetPosition() const;
		const glm::vec4& GetColor() const;
		const glm::vec2& GetDirection() const;
		const glm::vec2& GetLastDirection() const;
		const glm::vec3& GetLastTailPosition() const;

		void SetGalaxyID(const galaxy::api::GalaxyID& galaxyID);
		void SetPosition(const glm::vec3& position);
		void SetColor(const glm::vec4& color);
		void SetDirection(const glm::vec2& direction);
		void SetLastDirection(const glm::vec2& direction);
		void SetLastTailPosition(const glm::vec3& position);

		const std::vector<TailPartPtr> GetTail() const;
		void AddTailPart(const glm::vec3& position);

		void SetIsAlive(bool isAlive);
		bool IsAlive() const;

		void SetIsReady(bool isReady);
		bool IsReady() const;

		void SetPoints(std::uint32_t points);
		std::uint32_t GetPoints() const;

	private:

		galaxy::api::GalaxyID galaxyID;
		glm::vec3 position;
		glm::vec2 direction;
		glm::vec4 color;
		std::vector<TailPartPtr> tail;
		glm::vec3 lastTailPosition;
		glm::vec2 lastDirection;
		bool isAlive;
		bool isReady;
		std::int32_t points;
	};

	using PlayerPtr = std::shared_ptr<Player>;
	using PlayerPtrList = std::vector<PlayerPtr>;
}

#endif
