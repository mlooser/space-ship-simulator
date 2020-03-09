#pragma once

#include "TransformChangerComponent.h"
#include "EngineTypes.h"
#include "Component.h"
#include "Event.h"

namespace MustacheEngine
{
	class TransformRotatorComponent : public TransformChangerComponent
	{
	private:
		Vector3 m_RotationAxis;
		float m_RotationSpeed;
	public:
		TransformRotatorComponent() : TransformRotatorComponent(Vector3(0, 0, 1),0) {}
		TransformRotatorComponent(const Vector3& rotationAxis, float rotationSpeed);

		void SetRotationAxis(const Vector3& axis) { m_RotationAxis = axis; }
		void SetRotationSpeed(float rotationSpeed) { m_RotationSpeed = rotationSpeed; }

	protected:
		virtual void Update(float deltaTime) override;
	};
}