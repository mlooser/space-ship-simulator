#pragma once

#include "Component.h"
#include "EngineTypes.h"

namespace MustacheEngine
{
	class TransformComponent;

	class LightSourceComponent : public Component
	{
	private:
		TransformComponent* m_Transform;
	public:
		~LightSourceComponent();

		virtual void OnAttached(GameObject* owner) override;

		const Vector3& GetLightPosition() const;
	};
}
