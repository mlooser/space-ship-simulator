#pragma once

#include "Component.h"
#include "EngineTypes.h"
#include "Event.h"
#include "GameObject.h"//TOREMOVE
#include "ObjectsPool.h"

namespace MustacheEngine
{
	class GameObject;
	class TransformComponent;
	class ColliderComponent;
}

namespace SpaceShip
{
	class SpaceShipController : public MustacheEngine::Component
	{
	public:
		struct Config
		{
			float MaxSpeed = 2.0f;
			float Acceleration;
			float Deacceleration;
			float ShootingCooldown;
			float ProjectileSpeed;
			float ProjectileRange;
			float RollFollowInputSpeed;
			float MaxRoll;//degrees
			float XMovementLimit;
		};

	private:
		Config m_Config;

		MustacheEngine::Vector3 m_StartingLocation;
		float m_RequestedMovementDirection;
		float m_CurrentVelocity;
		bool m_ShootingRequested;
		float m_ElapsedShootingCooldown;
		float m_ProjectileDespawnY;

		MustacheEngine::TransformComponent* m_TransformComponent;

		MustacheEngine::EventBinding<float> m_UpdateBinding;
		MustacheEngine::EventBinding<MustacheEngine::ColliderComponent*, MustacheEngine::ColliderComponent*> m_CollisionBinding;

		std::unique_ptr<MustacheEngine::ObjectsPool<MustacheEngine::GameObject>> m_ProjectilesPool;
		std::vector<std::unique_ptr<MustacheEngine::GameObject>> m_Projectiles;

	public:
		SpaceShipController(const Config& cfg);
		~SpaceShipController();

		virtual void OnAttached(MustacheEngine::GameObject* owner) override;

		void RequestMovement(float movementDirection)
		{
			if (movementDirection == 0)
			{
				RequestStopMovement();
			}
			else
			{
				m_RequestedMovementDirection = glm::sign(movementDirection);
			}
		}

		void RequestStopMovement() { m_RequestedMovementDirection = 0; }

		void RequestShooting() { m_ShootingRequested = true; }
		void RequestStopShooting() { m_ShootingRequested = false; }
		void ResetState();
	private:
		void Update(float deltaTime);
		void UpdateMovement(float deltaTime);
		void UpdateShooting(float deltaTime);
		void UpdateProjectiles(float deltaTime);
		void UpdateRoll(float deltaTime);
		void SpawnProjectile(const MustacheEngine::Vector3& position);
		void SetupCollisionBinding();
		void HandleCollisionDetecion(MustacheEngine::ColliderComponent* first, MustacheEngine::ColliderComponent* second);
		void CleanupDespawnedProjectiles();
	};
}
