#pragma once

namespace SpaceShip
{
	enum class ECollisionLayer
	{
		None = 0,
		Projectile = 1 << 1,
		Meteor = 1 << 2,
		Ship = 1 << 3
	};
}