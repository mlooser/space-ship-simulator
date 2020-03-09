#pragma once

#include <memory>

#include "BaseGame.h"
#include "Event.h"

namespace MustacheEngine 
{
	class GameObject;
	class CameraComponent;
	class ConfigReader;
	class ColliderComponent;
}

namespace SpaceShip
{
	class ShipInputContext;

	class SpaceShipGame : public MustacheEngine::BaseGame
	{
	private:
		std::unique_ptr<MustacheEngine::GameObject> m_SpaceShip;
		std::unique_ptr<ShipInputContext> m_ShipInputContext;
		std::unique_ptr<MustacheEngine::GameObject> m_Camera;
		std::unique_ptr<MustacheEngine::GameObject> m_LightSource;
		std::unique_ptr<MustacheEngine::GameObject> m_MeteorsSpawner;

		MustacheEngine::EventBinding<MustacheEngine::ColliderComponent*, MustacheEngine::ColliderComponent*> m_CollisionBinding;
	public:
		SpaceShipGame();
		~SpaceShipGame();

		virtual void StartGame() override;
		virtual void ShutDown() override;
	private:
		
		void CreateCamera();
		void CreateLight();
		void CreateMeteorsSpawner(const MustacheEngine::ConfigReader& cfg);
		void CreateSpaceShip(const MustacheEngine::ConfigReader& cfg);
		void SetupInput();		

		void SetupCollisionDetection();
		void HandleCollisionDetecion(MustacheEngine::ColliderComponent* first, MustacheEngine::ColliderComponent* second);
	};
}
