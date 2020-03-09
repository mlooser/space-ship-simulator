#include "TransformMoverComponent.h"
#include "TransformComponent.h"

using namespace MustacheEngine;

void TransformMoverComponent::Update(float deltaTime)
{
	m_Transform->Move(m_MovementVelocity * deltaTime);
}