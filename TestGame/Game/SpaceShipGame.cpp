#include "SpaceShipGame.h"

#include "TransformComponent.h"
#include "GameObject.h"
#include "MeshRendererComponent.h"
#include "MeshFactory.h"
#include "MaterialInstance.h"
#include "MaterialInstanceFactory.h"
#include "InputSystem.h"
#include "CameraComponent.h"
#include "ConfigReader.h"
#include "LightSourceComponent.h"
#include "PhysicsSystem.h"
#include "ColliderComponent.h"
#include "SpaceShipGameConsts.h"
#include "TransformRotatorComponent.h"

#include "SpaceShipController.h"
#include "ShipInputContext.h"
#include "MeteorsController.h"
#include "ECollisionLayer.h"


using namespace MustacheEngine;
using namespace SpaceShip;
using namespace std;

SpaceShipGame::SpaceShipGame() = default;
SpaceShipGame::~SpaceShipGame() = default;

void SpaceShipGame::StartGame()
{
	SetTitle("Space Ship Simulator");

	ConfigReader cfg;
	cfg.ReadConfig("Resources/GameConfig.txt");

	CreateCamera();
	CreateLight();
	CreateSpaceShip(cfg);
	CreateMeteorsSpawner(cfg);
	SetupInput();
	SetupCollisionDetection();
}

void SpaceShipGame::ShutDown()
{
	m_ShipInputContext.release();
	m_SpaceShip.release();
	m_MeteorsSpawner.release();
	m_Camera.release();
	m_LightSource.release();
}

void SpaceShipGame::CreateSpaceShip(const ConfigReader& cfg)
{
	SpaceShipController::Config spaceShipCfg{};
	spaceShipCfg.MaxSpeed = cfg.GetFloat("SpaceShip.MaxSpeed");
	spaceShipCfg.Acceleration = cfg.GetFloat("SpaceShip.Acceleration");
	spaceShipCfg.Deacceleration = cfg.GetFloat("SpaceShip.Deacceleration");
	spaceShipCfg.ShootingCooldown = cfg.GetFloat("SpaceShip.ShootingCooldown");
	spaceShipCfg.ProjectileSpeed = cfg.GetFloat("SpaceShip.ProjectileSpeed");
	spaceShipCfg.RollFollowInputSpeed = cfg.GetFloat("SpaceShip.RollFollowInputSpeed");
	spaceShipCfg.ProjectileRange = cfg.GetFloat("SpaceShip.ProjectileRange");
	spaceShipCfg.MaxRoll = cfg.GetFloat("SpaceShip.MaxRoll");
	spaceShipCfg.XMovementLimit = cfg.GetFloat("SpaceShip.XMovementLimit");

	m_SpaceShip = std::make_unique<MustacheEngine::GameObject>();

	shared_ptr<Mesh>& shipMesh = GetMeshFactory()->LoadFromResource("Resources/Models/SpaceShip.obj");
	shared_ptr<MaterialInstance>& shipMaterialInstance = GetMaterialInstanceFactory()->LoadFromResource(SpaceShipGameConsts::MATERIAL_INSTANCE_SPACE_SHIP);

	m_SpaceShip
		->AddComponent<TransformComponent>(Vector3(0, -11, 0), Vector3(0, 0, 0), Vector3(0.7f, 0.7f, 0.7f))
		->AddComponent<SpaceShipController>(spaceShipCfg)
		->AddComponent<MeshRendererComponent>(shipMesh, shipMaterialInstance)
		->AddComponent<ColliderComponent>(CollisionShapeDescription{ CollisionShapeDescription::EShapeType::Box,{1.6f,1.1f,0.7f} });

	ColliderComponent* collider = m_SpaceShip->GetComponent<ColliderComponent>();
	collider->SetCollisionLayer((unsigned short)ECollisionLayer::Ship);
	collider->SetCollideWithLayers((unsigned short)ECollisionLayer::Meteor);

	m_SpaceShip->AddTag(SpaceShipGameConsts::SHIP_TAG);
}

void SpaceShipGame::SetupInput()
{
	SpaceShipController* shipController = m_SpaceShip->GetComponent<SpaceShipController>();
	m_ShipInputContext = std::make_unique<ShipInputContext>();
	m_ShipInputContext->SetControllerShip(shipController);
	GetSystem<InputSystem>()->PushInputContext(m_ShipInputContext.get());
}

void SpaceShipGame::CreateCamera()
{
	m_Camera = std::make_unique<MustacheEngine::GameObject>();

	m_Camera
		->AddComponent<TransformComponent>(Vector3(0, 0, 25))
		->AddComponent<CameraComponent>();
}

void SpaceShipGame::CreateLight()
{
	m_LightSource = std::make_unique<MustacheEngine::GameObject>();

	m_LightSource
		->AddComponent<TransformComponent>(Vector3(2, -8, 10))
		->AddComponent<LightSourceComponent>();
}

void SpaceShipGame::CreateMeteorsSpawner(const MustacheEngine::ConfigReader& cfg)
{
	MeteorsController::Config meteorsCfg{};
	meteorsCfg.InitialSpawningRate = cfg.GetFloat("Meteor.InitialSpawningRate");
	meteorsCfg.MeteorSpeed = cfg.GetFloat("Meteor.Speed");
	meteorsCfg.RotationRandomizationRange = cfg.GetFloat("Meteor.RotationRandomizationRange");
	meteorsCfg.SpawningRateIncrease = cfg.GetFloat("Meteor.SpawningRateIncrease");
	meteorsCfg.TargetRandomizationRange = cfg.GetFloat("Meteor.TargetRandomizationRange");

	m_MeteorsSpawner = std::make_unique<MustacheEngine::GameObject>();

	m_MeteorsSpawner
		->AddComponent<MeteorsController>(meteorsCfg);

	m_MeteorsSpawner
		->GetComponent<MeteorsController>()
		->SetupSpawning(Vector3(-14, 16, 0), Vector3(14, 17, 0), -17);

}

void SpaceShipGame::SetupCollisionDetection()
{
	m_CollisionBinding = std::move(
		BaseGame::GetInstance()->GetSystem<PhysicsSystem>()
		->OnCollisionDetected
		->Bind([this](ColliderComponent* first, ColliderComponent* second) { HandleCollisionDetecion(first, second); }));
}

void SpaceShipGame::HandleCollisionDetecion(ColliderComponent* first, ColliderComponent* second)
{
	GameObject* firstOwner = first->GetOwner();
	GameObject* secondOwner = second->GetOwner();
	
	bool shipWasHit = false;

	if (firstOwner->HasTag(SpaceShipGameConsts::SHIP_TAG) && secondOwner->HasTag(SpaceShipGameConsts::METEOR_TAG))
	{
		shipWasHit = true;
	}
	else if (secondOwner->HasTag(SpaceShipGameConsts::SHIP_TAG) && firstOwner->HasTag(SpaceShipGameConsts::METEOR_TAG))
	{
		shipWasHit = true;
	}

	if (shipWasHit)
	{
		m_SpaceShip->GetComponent<SpaceShipController>()->ResetState();
		m_MeteorsSpawner->GetComponent<MeteorsController>()->ResetState();
	}
}