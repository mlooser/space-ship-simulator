#pragma once

#include <vector>

#include "EInputKey.h"
#include "BaseSystem.h"

namespace MustacheEngine
{
	class InputContext;

	class InputSystem : public BaseSystem
	{
	private:
		GLFWwindow* m_Window;
		InputContext* m_ActiveContext;
		std::vector<InputContext*> m_ContextsStack;

	public:
		virtual void PostInitialize() override;
		virtual void Update() override;
		
		void PushInputContext(InputContext* context);		
		void RemoveInputContext(InputContext* context);

		bool IsKeyPressed(EInputKey key);
	};
}
