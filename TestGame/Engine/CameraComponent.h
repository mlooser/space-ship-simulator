#pragma once

#include "Component.h"

#include "EngineTypes.h"

namespace MustacheEngine
{
	class TransformComponent;

	class CameraComponent : public Component
	{
	private:
		TransformComponent* m_Transform;
		float m_FOV;
		float m_Aspect;
		float m_NearPlane;
		float m_FarPlane;

	public:
		CameraComponent(float fov = 45.0f, float aspect = 4.0f/3.0f, float nearPlane = 0.1f, float farPlane = 100.0f);
		~CameraComponent();

		virtual void OnAttached(GameObject* owner) override;

		Matrix4 CalculateViewProjection() const;

		Matrix4 CalculateProjection() const;
		Matrix4 CalculateView() const;
	};
}
