#ifndef GOGTRON_RENDERER_CAMERA_H
#define GOGTRON_RENDERER_CAMERA_H

#include <glm/vec3.hpp>

namespace gogtron
{
	namespace renderer
	{

		class Camera
		{
		public:

			glm::vec3 m_vPosition;
			glm::vec3 m_vView;
			glm::vec3 m_vUpVector;

			Camera();

			void PositionCamera(float positionX, float positionY, float positionZ,
				float viewX, float viewY, float viewZ,
				float upVectorX, float upVectorY, float upVectorZ);

			void RotateView(float angle, float X, float Y, float Z);

			void RotateAroundPoint(const glm::vec3& vCenter, float angle, float x, float y, float z);

			void MoveCamera(float speed);
		};

	}
}

#endif