#include "CameraComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "BaseGame.h"
#include "RenderingSystem.h"
#include "GameObject.h"
#include "TransformComponent.h"

using namespace MustacheEngine;

CameraComponent::CameraComponent(float fov, float aspect, float nearPlane, float farPlane)
	: m_FOV(fov)
	, m_Aspect(aspect)
	, m_NearPlane(nearPlane)
	, m_FarPlane(farPlane)
{

}

CameraComponent::~CameraComponent()
{
	BaseGame::GetInstance()->GetSystem<RenderingSystem>()->SetActiveCamera(nullptr);
}

void CameraComponent::OnAttached(GameObject* owner)
{
	Component::OnAttached(owner);

	m_Transform = m_Owner->GetComponent<TransformComponent>();

	BaseGame::GetInstance()->GetSystem<RenderingSystem>()->SetActiveCamera(this);
}

Matrix4 CameraComponent::CalculateViewProjection() const
{

	return CalculateProjection() * CalculateView();
}

Matrix4 CameraComponent::CalculateProjection() const
{
	Matrix4 projectionMatrix = glm::perspective(m_FOV, m_Aspect, m_NearPlane, m_FarPlane);
	return projectionMatrix;
}

Matrix4 CameraComponent::CalculateView() const
{
	float verticalAngle = 0;
	float horizontalAngle = 3.14f;

	Vector3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	Vector3 right = Vector3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
		);

	Vector3 up = glm::cross(right, direction);


	const Vector3& position = m_Transform->GetPosition();

	Matrix4 viewMatrix = glm::lookAt(position, position + direction, up);
	return viewMatrix;
}