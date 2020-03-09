#pragma once

#include "TransformChangerComponent.h"
#include "EngineTypes.h"
#include "Component.h"
#include "Event.h"

namespace MustacheEngine
{
	class TransformMoverComponent : public TransformChangerComponent
	{
	private:
		Vector3 m_MovementVelocity;

	public:
		TransformMoverComponent() : TransformMoverComponent(Vector3(0, 0, 0)) {}
		TransformMoverComponent(const Vector3& velocity) : m_MovementVelocity(velocity) {}

		void SetMovementVelocity(const Vector3& velocity) { m_MovementVelocity = velocity; }

	protected:
		virtual void Update(float deltaTime) override;
	};
}