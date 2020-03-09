#include "InputContext.h"

#include "InputSystem.h"

using namespace MustacheEngine;

InputContext::~InputContext()
{
	m_InputSystem->RemoveInputContext(this);
}

bool InputContext::IsKeyPressed(EInputKey key)
{
	return m_InputSystem->IsKeyPressed(key);
}