#pragma once

#include "EngineTypes.h"
#include "Component.h"
#include "Event.h"

namespace MustacheEngine
{
	class TransformComponent;

	class TransformChangerComponent : public Component
	{
	protected:
		TransformComponent* m_Transform;
		EventBinding<float> m_UpdateBinding;

	public:
		virtual void OnAttached(GameObject* owner) override;

	protected:
		virtual void Update(float deltaTime) = 0;
		virtual void Activate() override;
		virtual void Deactivate() override;
	};
}