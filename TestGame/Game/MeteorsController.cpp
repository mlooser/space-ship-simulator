#include "MeteorsController.h"

#include <random>
#include <array>
#include <utility>

#include "GameObject.h"
#include "Mesh.h"
#include "BaseGame.h"
#include "MaterialInstance.h"
#include "MeshFactory.h"
#include "MaterialInstanceFactory.h"
#include "TransformComponent.h"
#include "TransformMoverComponent.h"
#include "TransformRotatorComponent.h"
#include "MeshRendererComponent.h"
#include "UpdateManager.h"
#include "PhysicsSystem.h"
#include "ColliderComponent.h"

#include "SpaceShipGameConsts.h"
#include "ECollisionLayer.h"

using namespace MustacheEngine;
using namespace SpaceShip;

static std::default_random_engine RANDOM_ENGINE;

MeteorsController::MeteorsController(const MeteorsController::Config& cfg)
	: m_Config(cfg)
{
	auto meteorFacotryFunc = [this]()
	{
		GameObject* meteor = new GameObject();

		std::shared_ptr<Mesh>& projectileMesh = BaseGame::GetInstance()->GetMeshFactory()->LoadFromResource("Resources/Models/Meteor.obj");
		std::shared_ptr<MaterialInstance>& projectileMaterial = BaseGame::GetInstance()->GetMaterialInstanceFactory()->LoadFromResource(SpaceShipGameConsts::MATERIAL_INSTANCE_METEOR);

		meteor
			->AddComponent<TransformComponent>()
			->AddComponent<MeshRendererComponent>(projectileMesh, projectileMaterial)
			->AddComponent<TransformMoverComponent>()
			->AddComponent<TransformRotatorComponent>(Vector3(0, 1, 0), 0)
			->AddComponent<ColliderComponent>();

		ColliderComponent* collider = meteor->GetComponent<ColliderComponent>();
		collider->SetCollisionLayer((unsigned short)ECollisionLayer::Meteor);
		collider->SetCollideWithLayers((unsigned short)ECollisionLayer::Projectile | (unsigned short)ECollisionLayer::Ship);

		meteor->AddTag(SpaceShipGameConsts::METEOR_TAG);

		return meteor;
	};

	m_MeteorsPool = std::make_unique<ObjectsPool<GameObject>>(meteorFacotryFunc);

	m_SpawningRate = cfg.InitialSpawningRate;
	RefreshSpawningInterval();
	m_TimeToNextSpawn = m_SpawningInterval;
}

MeteorsController::~MeteorsController() = default;

void MeteorsController::OnAttached(MustacheEngine::GameObject* owner)
{
	Component::OnAttached(owner);

	SetupUpdateBinding();
	SetupCollisionBinding();
}

void MeteorsController::SetupUpdateBinding()
{
	m_UpdateBinding = std::move(
		BaseGame::GetInstance()->GetSystem<UpdateManager>()
		->OnUpdate
		->Bind([this](float dt) {Update(dt); }));
}

void MeteorsController::SetupCollisionBinding()
{
	m_CollisionBinding = std::move(
		BaseGame::GetInstance()->GetSystem<PhysicsSystem>()
		->OnCollisionDetected
		->Bind([this](ColliderComponent* first, ColliderComponent* second) { HandleCollisionDetecion(first, second); }));
}

void MeteorsController::HandleCollisionDetecion(ColliderComponent* first, ColliderComponent* second)
{
	GameObject* firstOwner = first->GetOwner();
	GameObject* secondOwner = second->GetOwner();
	GameObject* meteorToDestroy = nullptr;

	if ((firstOwner->HasTag(SpaceShipGameConsts::METEOR_TAG) && secondOwner->HasTag(SpaceShipGameConsts::PROJECTILE_TAG)))
	{
		meteorToDestroy = firstOwner;
	}
	else if (secondOwner->HasTag(SpaceShipGameConsts::METEOR_TAG) && firstOwner->HasTag(SpaceShipGameConsts::PROJECTILE_TAG))
	{
		meteorToDestroy = secondOwner;
	}

	if (meteorToDestroy)
	{
		m_SpawningRate += m_Config.SpawningRateIncrease;
		RefreshSpawningInterval();

		meteorToDestroy->SetActive(false);
		for (int i = 0; i < m_Meteors.size(); ++i)
		{
			std::unique_ptr<GameObject>& go = m_Meteors[i];
			if (go.get() == meteorToDestroy)
			{
				go->SetActive(false);
				m_MeteorsPool->GiveBackObject(std::move(go));
				break;
			}
		}

		CleanupDespawnedMeteors();
	}
}

void MeteorsController::Update(float deltaTime)
{
	UpdateSpawning(deltaTime);
	UpdateDespawning(deltaTime);
}

void MeteorsController::UpdateSpawning(float deltaTime)
{
	m_TimeToNextSpawn -= deltaTime;
	if (m_TimeToNextSpawn > 0)
		return;

	m_TimeToNextSpawn = m_SpawningInterval;
	std::unique_ptr<GameObject> meteor = std::move(m_MeteorsPool->GetObject());

	meteor->SetActive(true);
	RandomizeMeteor(meteor.get());
	m_Meteors.push_back(std::move(meteor));
}

void MeteorsController::UpdateDespawning(float deltaTime)
{
	bool anyRemoved = false;

	for (int i = 0; i < m_Meteors.size(); ++i)
	{
		std::unique_ptr<GameObject>& go = m_Meteors[i];
		TransformComponent* transform = go->GetComponent<TransformComponent>();
		if (transform->GetPosition().y < m_TargetY)
		{
			anyRemoved = true;
			go->SetActive(false);
			m_MeteorsPool->GiveBackObject(std::move(go));
		}
	}

	if (!anyRemoved)
		return;

	CleanupDespawnedMeteors();
}

void MeteorsController::CleanupDespawnedMeteors()
{
	m_Meteors.erase(std::remove_if(m_Meteors.begin(), m_Meteors.end(), [this](const auto& p)
	{
		return p.get() == nullptr;
	}), m_Meteors.end());
}

void MeteorsController::RandomizeMeteor(MustacheEngine::GameObject* meteor)
{
	SetupRandomMeteorMovement(meteor);
	RandomizeMeteorSize(meteor);
}

void MeteorsController::SetupRandomMeteorMovement(GameObject* meteor)
{
	static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

	float rand = distribution(RANDOM_ENGINE);
	Vector3 src = m_MeteorSpawningMin + rand * (m_MeteorSpawningMax - m_MeteorSpawningMin);
	rand = distribution(RANDOM_ENGINE);
	float dstX = src.x + (rand - 0.5f) * 2 * m_Config.TargetRandomizationRange;
	dstX = glm::clamp(dstX, m_MeteorSpawningMin.x, m_MeteorSpawningMax.x);
	Vector3 dst(dstX, m_TargetY, src.z);
	Vector3 movementDirection = glm::normalize(dst - src);

	meteor->GetComponent<TransformMoverComponent>()->SetMovementVelocity(movementDirection* m_Config.MeteorSpeed);
	meteor->GetComponent<TransformComponent>()->SetPosition(src);
	meteor->GetComponent<TransformComponent>()->SetRotation(Vector3(0, 0, 1));

	float rotation = glm::radians(m_Config.RotationRandomizationRange) * (distribution(RANDOM_ENGINE) - 0.5f) * 2;

	meteor->GetComponent<TransformRotatorComponent>()->SetRotationSpeed(rotation);
}

void MeteorsController::RandomizeMeteorSize(MustacheEngine::GameObject* meteor)
{
	using Preset = std::pair<Vector3, CollisionShapeDescription>;//<scale,collision shape>

	static std::array<Preset, 7> presets =
	{
		Preset(Vector3{ 1.0f,1.0f,1.0f },CollisionShapeDescription{ CollisionShapeDescription::EShapeType::Box,Vector3{ 1.0f,1.0f,1.0f } }),

		Preset(Vector3{ 1.5f,1.0f,1.0f },CollisionShapeDescription{ CollisionShapeDescription::EShapeType::Box,Vector3{ 1.5f,1.0f,1.0f } }),
		Preset(Vector3{ 1.5f,1.5f,1.0f },CollisionShapeDescription{ CollisionShapeDescription::EShapeType::Box,Vector3{ 1.5f,1.5f,1.0f } }),
		Preset(Vector3{ 1.5f,1.5f,1.5f },CollisionShapeDescription{ CollisionShapeDescription::EShapeType::Box,Vector3{ 1.5f,1.5f,1.5f } }),

		Preset(Vector3{ 2.0f,1.5f,1.5f },CollisionShapeDescription{ CollisionShapeDescription::EShapeType::Box,Vector3{ 2.0f,1.5f,1.5f } }),
		Preset(Vector3{ 2.0f,2.0f,1.5f },CollisionShapeDescription{ CollisionShapeDescription::EShapeType::Box,Vector3{ 2.0f,2.0f,1.5f } }),
		Preset(Vector3{ 2.0f,2.0f,2.0f },CollisionShapeDescription{ CollisionShapeDescription::EShapeType::Box,Vector3{ 2.0f,2.0f,2.0f } })
	};

	static std::uniform_int_distribution<int> distribution(0, presets.size() - 1);

	Preset& preset = presets[distribution(RANDOM_ENGINE)];

	meteor->GetComponent<TransformComponent>()->SetScale(preset.first);
	meteor->GetComponent<ColliderComponent>()->SetShapeDescription(preset.second);
}

void MeteorsController::ResetState()
{
	std::for_each(
		m_Meteors.begin(),
		m_Meteors.end(),
		[this](std::unique_ptr<GameObject>& go)
	{
		go->SetActive(false);
		m_MeteorsPool->GiveBackObject(std::move(go));
	});

	CleanupDespawnedMeteors();

	m_SpawningRate = m_Config.InitialSpawningRate;
	RefreshSpawningInterval();
}