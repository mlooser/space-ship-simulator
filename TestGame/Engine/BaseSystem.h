#pragma once

#include "EngineMacros.h"

namespace MustacheEngine
{
	class BaseGame;

	class BaseSystem
	{
	protected:
		BaseGame* m_Owner;

	public:
		BaseSystem() = default;

		PREVENT_COPY_AND_MOVE(BaseSystem);

		virtual ~BaseSystem() = default;
		virtual void Initialize(BaseGame* owner) { m_Owner = owner; }
		virtual void PostInitialize() {}
		virtual void Update() {}
		virtual void Shutdown() {}
	};
}
