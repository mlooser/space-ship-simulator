#include <iostream>

#include <algorithm>

#include "InputSystem.h"
#include "Logger.h"
#include "BaseGame.h"
#include "RenderingSystem.h"
#include "InputContext.h"

using namespace MustacheEngine;

bool InputSystem::IsKeyPressed(EInputKey key)
{
	return glfwGetKey(m_Window, (int)key) == GLFW_PRESS;
}

void InputSystem::PostInitialize()
{
	m_Window = m_Owner->GetSystem<RenderingSystem>()->GetWindow();

	glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GL_TRUE);
	if (m_Owner->GetConfig().HideCursor)
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

}

void InputSystem::PushInputContext(InputContext* context)
{
	if (m_ActiveContext != nullptr)
	{
		m_ActiveContext->Deactivate();
		m_ContextsStack.push_back(m_ActiveContext);
	}

	m_ActiveContext = context;
	if (m_ActiveContext != nullptr)
	{
		m_ActiveContext->Activated(this);
	}
}

void InputSystem::RemoveInputContext(InputContext* context)
{
	if (m_ActiveContext == context)
	{
		m_ActiveContext->Deactivate();

		if (m_ContextsStack.size() > 0)
		{
			m_ActiveContext = m_ContextsStack[m_ContextsStack.size() - 1];
			m_ContextsStack.pop_back();
			m_ActiveContext->Activated(this);
		}
		else
		{
			m_ActiveContext = nullptr;
		}
	}
	else
	{
		m_ContextsStack.erase(std::remove(m_ContextsStack.begin(), m_ContextsStack.end(), context), m_ContextsStack.end());
	}
}


void InputSystem::Update()
{
	glfwPollEvents();

	if (m_ActiveContext != nullptr)
	{
		m_ActiveContext->Update();
	}
}