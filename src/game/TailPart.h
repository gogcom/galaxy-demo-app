#ifndef GOGTRON_TAIL_PART_H
#define GOGTRON_TAIL_PART_H

#include "GameObject.h"
#include <memory>

namespace gogtron
{

	class TailPart : public GameObject
	{
	public:

		TailPart(const glm::vec3& position, const glm::vec4& color);

		const glm::vec4& GetColor() const;

		void SetColor(const glm::vec4& color);

	private:

		glm::vec4 color;
	};

	using TailPartPtr = std::shared_ptr<TailPart>;

}

#endif