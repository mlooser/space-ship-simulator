#include "LightSourceComponent.h"

#include "BaseGame.h"
#include "RenderingSystem.h"
#include "GameObject.h"
#include "TransformComponent.h"

using namespace MustacheEngine;

LightSourceComponent::~LightSourceComponent()
{
	BaseGame::GetInstance()->GetSystem<RenderingSystem>()->SetActiveLightSource(nullptr);
}

void LightSourceComponent::OnAttached(GameObject* owner)
{
	Component::OnAttached(owner);

	m_Transform = owner->GetComponent<TransformComponent>();

	BaseGame::GetInstance()->GetSystem<RenderingSystem>()->SetActiveLightSource(this);
}

const Vector3& LightSourceComponent::GetLightPosition() const
{
	return m_Transform->GetPosition();
}