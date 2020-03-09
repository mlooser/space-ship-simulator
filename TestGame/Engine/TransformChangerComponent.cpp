#include "TransformChangerComponent.h"
#include "GameObject.h"
#include "BaseGame.h"
#include "UpdateManager.h"
#include "TransformComponent.h"

using namespace MustacheEngine;

void TransformChangerComponent::OnAttached(GameObject* owner)
{
	Component::OnAttached(owner);
	m_Transform = owner->GetComponent<TransformComponent>();
}

void TransformChangerComponent::Activate()
{
	m_UpdateBinding = std::move(
		BaseGame::GetInstance()->GetSystem<UpdateManager>()
		->OnUpdate
		->Bind([this](float dt) {Update(dt); }));
}

void TransformChangerComponent::Deactivate()
{
	m_UpdateBinding.Unbind();
}