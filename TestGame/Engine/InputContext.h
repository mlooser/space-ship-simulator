#pragma once

#include "EInputKey.h"

namespace MustacheEngine
{
	class InputSystem;

	class InputContext
	{
	private:
		InputSystem* m_InputSystem;

	public:
		virtual void Activated(InputSystem* inputSystem) { m_InputSystem = inputSystem; }
		virtual void Deactivate() {}
		virtual void Update() {}

		virtual ~InputContext();

	protected:
		bool IsKeyPressed(EInputKey key);
	};
}
