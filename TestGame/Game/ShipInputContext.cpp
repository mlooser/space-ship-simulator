#include "ShipInputContext.h"

#include "EngineTypes.h"

#include "SpaceShipController.h"

using namespace MustacheEngine;
using namespace SpaceShip;

void ShipInputContext::Update()
{
	float movementDir = 0;
	if (IsKeyPressed(EInputKey::KeyA))
	{
		movementDir -= 1;
	}
	if (IsKeyPressed(EInputKey::KeyD))
	{
		movementDir += 1;
	}

	m_ShipController->RequestMovement(movementDir);

	if (IsKeyPressed(EInputKey::KeySpace))
	{
		m_ShipController->RequestShooting();
	}
	else
	{
		m_ShipController->RequestStopShooting();
	}
}