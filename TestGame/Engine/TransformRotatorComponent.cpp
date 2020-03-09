#include "TransformRotatorComponent.h"

#include <glm/gtx/quaternion.hpp>

#include "TransformComponent.h"

using namespace MustacheEngine;

TransformRotatorComponent::TransformRotatorComponent(const Vector3& rotationAxis, float rotationSpeed)
	: m_RotationAxis(rotationAxis)
	, m_RotationSpeed(rotationSpeed)
{
	
}

void TransformRotatorComponent::Update(float deltaTime)
{
	Quaternion rotation = glm::angleAxis(m_RotationSpeed * deltaTime, m_RotationAxis);
	m_Transform->Rotate(rotation);
}