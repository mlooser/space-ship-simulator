#include "TransformComponent.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

using namespace MustacheEngine;

Matrix4 TransformComponent::CalculateModelMatrix() const
{
	Matrix4 rotationMatrix = glm::toMat4(m_Rotation);
	Matrix4 translationMatrix = glm::translate(glm::mat4(1.0f), m_Position);
	Matrix4 scaleMatrix = glm::scale(m_Scale);

	return translationMatrix * rotationMatrix * scaleMatrix;
}