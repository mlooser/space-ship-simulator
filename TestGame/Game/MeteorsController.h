#pragma once

#include <memory>
#include <vector>

#include "Component.h"
#include "EngineTypes.h"
#include "Event.h"
#include "GameObject.h"
#include "ObjectsPool.h"

namespace MustacheEngine 
{
	class GameObject;	
	class ColliderComponent;
}

namespace SpaceShip
{
	class MeteorsController : public MustacheEngine::Component
	{
	public:
		struct Config
		{
			float MeteorSpeed;
			float InitialSpawningRate; //meteors per second
			float SpawningRateIncrease;
			float TargetRandomizationRange;
			float RotationRandomizationRange;
		};

	private:
		using Vector3 = MustacheEngine::Vector3;

		Config m_Config;

		Vector3 m_MeteorSpawningMin;
		Vector3 m_MeteorSpawningMax;

		float m_TargetY;

		MustacheEngine::EventBinding<float> m_UpdateBinding;
		MustacheEngine::EventBinding<MustacheEngine::ColliderComponent*, MustacheEngine::ColliderComponent*> m_CollisionBinding;

		std::unique_ptr<MustacheEngine::ObjectsPool<MustacheEngine::GameObject>> m_MeteorsPool;
		std::vector<std::unique_ptr<MustacheEngine::GameObject>> m_Meteors;
		
		float m_SpawningInterval;
		float m_SpawningRate;
		float m_TimeToNextSpawn;

	public:
		MeteorsController(const Config& cfg);
		~MeteorsController();

		virtual void OnAttached(MustacheEngine::GameObject* owner) override;

		void SetupSpawning(const Vector3& meteorSpawningMin, const Vector3& meteorSpawningMax, float targetY)
		{
			m_MeteorSpawningMin = meteorSpawningMin;
			m_MeteorSpawningMax = meteorSpawningMax;
			m_TargetY = targetY;
		}

		void ResetState();

	private:
		void RefreshSpawningInterval() { m_SpawningInterval = 1 / m_SpawningRate; }
		void SetupUpdateBinding();
		void SetupCollisionBinding();
		void HandleCollisionDetecion(MustacheEngine::ColliderComponent* first, MustacheEngine::ColliderComponent* second);
		
		void Update(float deltaTime);
		void UpdateSpawning(float deltaTime);
		void UpdateDespawning(float deltaTime);
		
		void CleanupDespawnedMeteors();

		void SetupRandomMeteorMovement(MustacheEngine::GameObject* meteor);
		void RandomizeMeteorSize(MustacheEngine::GameObject* meteor);
		void RandomizeMeteor(MustacheEngine::GameObject* meteor);
	};
}