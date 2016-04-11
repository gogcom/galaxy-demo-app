#include "Camera.h"
#include <cmath>

using namespace gogtron::renderer;

Camera::Camera()
{
	glm::vec3 vZero = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 vView = glm::vec3(0.0, 1.0, 0.5);
	glm::vec3 vUp = glm::vec3(0.0, 0.0, 1.0);

	m_vPosition = vZero;
	m_vView = vView;
	m_vUpVector = vUp;
}

void Camera::PositionCamera(float positionX, float positionY, float positionZ,
	float viewX, float viewY, float viewZ,
	float upVectorX, float upVectorY, float upVectorZ)
{
	glm::vec3 vPosition = glm::vec3(positionX, positionY, positionZ);
	glm::vec3 vView = glm::vec3(viewX, viewY, viewZ);
	glm::vec3 vUpVector = glm::vec3(upVectorX, upVectorY, upVectorZ);

	m_vPosition = vPosition;
	m_vView = vView;
	m_vUpVector = vUpVector;
}


void Camera::RotateView(float angle, float x, float y, float z)
{
	glm::vec3 vNewView;
	glm::vec3 vView = m_vView - m_vPosition;

	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	vNewView.x = (cosTheta + (1 - cosTheta) * x * x)		* vView.x;
	vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.y;
	vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.z;

	vNewView.y = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * y * y)		* vView.y;
	vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.z;

	vNewView.z = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.x;
	vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * z * z)		* vView.z;

	m_vView = m_vPosition + vNewView;
}

void Camera::RotateAroundPoint(const glm::vec3& vCenter, float angle, float x, float y, float z)
{
	glm::vec3 vNewPosition;
	glm::vec3 vPos = m_vPosition - vCenter;

	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	vNewPosition.x = (cosTheta + (1 - cosTheta) * x * x)		* vPos.x;
	vNewPosition.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vPos.y;
	vNewPosition.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vPos.z;

	vNewPosition.y = ((1 - cosTheta) * x * y + z * sinTheta)	* vPos.x;
	vNewPosition.y += (cosTheta + (1 - cosTheta) * y * y)		* vPos.y;
	vNewPosition.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vPos.z;

	vNewPosition.z = ((1 - cosTheta) * x * z - y * sinTheta)	* vPos.x;
	vNewPosition.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vPos.y;
	vNewPosition.z += (cosTheta + (1 - cosTheta) * z * z)		* vPos.z;

	m_vPosition = vCenter + vNewPosition;
}

void Camera::MoveCamera(float speed)
{
	glm::vec3 vVector = m_vView - m_vPosition;

	m_vPosition.x += vVector.x * speed;
	m_vPosition.z += vVector.z * speed;
	m_vView.x += vVector.x * speed;
	m_vView.z += vVector.z * speed;
}