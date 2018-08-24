#ifndef GALAXY_DEMO_GAME_OBJECT_H
#define GALAXY_DEMO_GAME_OBJECT_H

#include <glm/glm.hpp>
#include <cstdint>

namespace galaxy::demo
{

	class GameObject
	{
	public:

		GameObject(const glm::vec3& position);

		const glm::vec3& GetPosition() const;
		std::uint32_t GetId() const;

		void SetPosition(const glm::vec3& position);

	protected:

		glm::vec3 position;
		std::uint32_t id;

	private:

		static std::uint32_t GameObjectId;
	};

}

#endif