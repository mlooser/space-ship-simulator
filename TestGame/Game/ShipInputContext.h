#pragma once

#include "InputContext.h"

namespace SpaceShip
{
	class SpaceShipController;

	class ShipInputContext : public MustacheEngine::InputContext
	{
	private:
		SpaceShipController* m_ShipController;

	public:
		void SetControllerShip(SpaceShipController* shipController) { m_ShipController = shipController; };
		virtual void Update() override;
	};
}
