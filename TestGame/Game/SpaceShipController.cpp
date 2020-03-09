#include "SpaceShipController.h"

#include <algorithm>

#include "BaseGame.h"
#include "UpdateManager.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "TransformMoverComponent.h"
#include "MeshFactory.h"
#include "MaterialInstanceFactory.h"
#include "TransformRotatorComponent.h"
#include "ColliderComponent.h"
#include "PhysicsSystem.h"

#include "SpaceShipGameConsts.h"
#include "ECollisionLayer.h"

using namespace MustacheEngine;
using namespace SpaceShip;

SpaceShipController::~SpaceShipController() = default;

SpaceShipController::SpaceShipController(const Config& cfg)
	: m_Config(cfg)
	, m_RequestedMovementDirection(0)
	, m_CurrentVelocity(0)
	, m_ShootingRequested(false)
	, m_ElapsedShootingCooldown(cfg.ShootingCooldown)
{
	auto projectileFacotryFunc = [this]()
	{
		GameObject* projectile = new GameObject();

		std::shared_ptr<Mesh>& projectileMesh = BaseGame::GetInstance()->GetMeshFactory()->LoadFromResource("Resources/Models/Projectile.obj");
		std::shared_ptr<MaterialInstance>& projectileMaterial = BaseGame::GetInstance()->GetMaterialInstanceFactory()->LoadFromResource(SpaceShipGameConsts::MATERIAL_INSTANCE_PROJECTILE);

		projectile
			->AddComponent<TransformComponent>()
			->AddComponent<MeshRendererComponent>(projectileMesh, projectileMaterial)
			->AddComponent<TransformMoverComponent>(Vector3(0, m_Config.ProjectileSpeed, 0))
			->AddComponent<TransformRotatorComponent>(Vector3(0, 1, 0), 4.14f)
			->AddComponent<ColliderComponent>(CollisionShapeDescription{ CollisionShapeDescription::EShapeType::Box,{0.15f,0.5f,0.15f} });

		ColliderComponent* collider = projectile->GetComponent<ColliderComponent>();
		collider->SetCollisionLayer((unsigned short)ECollisionLayer::Projectile);
		collider->SetCollideWithLayers((unsigned short)ECollisionLayer::Meteor);

		projectile->AddTag(SpaceShipGameConsts::PROJECTILE_TAG);

		projectile->GetComponent<TransformComponent>()->SetScale({ 0.3f,1,0.3f });

		return projectile;
	};

	m_ProjectilesPool = std::make_unique<ObjectsPool<GameObject>>(projectileFacotryFunc);
}

void SpaceShipController::OnAttached(GameObject* owner)
{
	Component::OnAttached(owner);

	m_TransformComponent = owner->GetComponent<TransformComponent>();

	m_UpdateBinding = std::move(
		BaseGame::GetInstance()->GetSystem<UpdateManager>()
		->OnUpdate
		->Bind([this](float dt) {Update(dt); }));

	m_StartingLocation = m_TransformComponent->GetPosition();
	m_ProjectileDespawnY = m_StartingLocation.y + m_Config.ProjectileRange;

	SetupCollisionBinding();
}

void SpaceShipController::SetupCollisionBinding()
{
	m_CollisionBinding = std::move(
		BaseGame::GetInstance()->GetSystem<PhysicsSystem>()
		->OnCollisionDetected
		->Bind([this](ColliderComponent* first, ColliderComponent* second) { HandleCollisionDetecion(first, second); }));
}


void SpaceShipController::HandleCollisionDetecion(ColliderComponent* first, ColliderComponent* second)
{
	GameObject* firstOwner = first->GetOwner();
	GameObject* secondOwner = second->GetOwner();
	GameObject* projectileToDestroy = nullptr;

	if ((firstOwner->HasTag(SpaceShipGameConsts::METEOR_TAG) && secondOwner->HasTag(SpaceShipGameConsts::PROJECTILE_TAG)))
	{
		projectileToDestroy = secondOwner;
	}
	else if (secondOwner->HasTag(SpaceShipGameConsts::METEOR_TAG) && firstOwner->HasTag(SpaceShipGameConsts::PROJECTILE_TAG))
	{
		projectileToDestroy = firstOwner;
	}

	if (projectileToDestroy)
	{
		projectileToDestroy->SetActive(false);

		for (int i = 0; i < m_Projectiles.size(); ++i)
		{
			std::unique_ptr<GameObject>& go = m_Projectiles[i];

			if (go.get() == projectileToDestroy)
			{
				m_ProjectilesPool->GiveBackObject(std::move(go));
			}
		}

		CleanupDespawnedProjectiles();
	}
}

void SpaceShipController::Update(float deltaTime)
{
	UpdateMovement(deltaTime);
	UpdateRoll(deltaTime);
	UpdateShooting(deltaTime);
	UpdateProjectiles(deltaTime);
}

void SpaceShipController::UpdateMovement(float deltaTime)
{
	if (m_CurrentVelocity == 0 && m_RequestedMovementDirection == 0)
		return;

	float originalV = m_CurrentVelocity;
	float acc = 0;

	if (m_RequestedMovementDirection != 0)
	{
		acc = m_RequestedMovementDirection*m_Config.Acceleration;
		m_CurrentVelocity += acc * deltaTime;
		m_CurrentVelocity = glm::clamp(m_CurrentVelocity, -m_Config.MaxSpeed, m_Config.MaxSpeed);

		if (glm::sign(m_CurrentVelocity) == glm::sign(m_RequestedMovementDirection) && glm::abs(m_CurrentVelocity) >= m_Config.MaxSpeed) //max speed reached
		{
			acc = 0;
		}
	}
	else
	{
		acc = -glm::sign(m_CurrentVelocity)*m_Config.Deacceleration;
		float velocityChange = acc * deltaTime;
		m_CurrentVelocity += velocityChange;
		if (glm::sign(m_CurrentVelocity) == glm::sign(acc))//velocity changes sign after deacceleration
		{
			m_CurrentVelocity = 0;
		}
	}

	Vector3 move = Vector3(originalV * deltaTime + acc*deltaTime*deltaTime / 2, 0, 0);
	Vector3 position = m_TransformComponent->GetPosition() + move;
	position.x = glm::clamp(position.x, -m_Config.XMovementLimit, m_Config.XMovementLimit);

	m_TransformComponent->SetPosition(position);
}

void SpaceShipController::UpdateRoll(float deltaTime)
{
	const Quaternion& currentRot = m_TransformComponent->GetRotation();

	float targetAngle = glm::sign(m_RequestedMovementDirection) * glm::radians(m_Config.MaxRoll);
	Quaternion targetRot(Vector3(0.0f, targetAngle, 0.0f));
	float lerp = glm::clamp(deltaTime * m_Config.RollFollowInputSpeed, 0.0f, 1.0f);
	targetRot = glm::lerp(currentRot, targetRot, lerp);

	m_TransformComponent->SetRotation(targetRot);
}

void SpaceShipController::UpdateShooting(float deltaTime)
{
	if (m_ElapsedShootingCooldown < m_Config.ShootingCooldown)
	{
		m_ElapsedShootingCooldown += deltaTime;
	}

	if (m_ShootingRequested && m_ElapsedShootingCooldown >= m_Config.ShootingCooldown)
	{
		m_ElapsedShootingCooldown = 0;
		SpawnProjectile(m_TransformComponent->GetPosition());
	}
}

void SpaceShipController::SpawnProjectile(const Vector3& position)
{
	std::unique_ptr<GameObject> projectile = std::move(m_ProjectilesPool->GetObject());

	projectile->SetActive(true);
	projectile->GetComponent<TransformComponent>()->SetPosition(position + Vector3(0, 0.5f, 0));
	m_Projectiles.push_back(std::move(projectile));
}

void SpaceShipController::UpdateProjectiles(float deltaTime)
{
	bool anyRemoved = false;

	for (int i = 0; i < m_Projectiles.size(); ++i)
	{
		std::unique_ptr<GameObject>& go = m_Projectiles[i];
		TransformComponent* transform = go->GetComponent<TransformComponent>();
		if (transform->GetPosition().y > m_ProjectileDespawnY)
		{
			anyRemoved = true;
			go->SetActive(false);
			m_ProjectilesPool->GiveBackObject(std::move(go));
		}
	}

	if (!anyRemoved)
		return;

	CleanupDespawnedProjectiles();
}

void SpaceShipController::CleanupDespawnedProjectiles()
{
	m_Projectiles.erase(std::remove_if(m_Projectiles.begin(), m_Projectiles.end(), [this](const auto& p)
	{
		return p.get() == nullptr;
	}), m_Projectiles.end());
}

void SpaceShipController::ResetState()
{
	std::for_each(
		m_Projectiles.begin(), 
		m_Projectiles.end(), 
		[this](std::unique_ptr<GameObject>& go) 
	{
		go->SetActive(false);
		m_ProjectilesPool->GiveBackObject(std::move(go));
	});

	CleanupDespawnedProjectiles();

	m_TransformComponent->SetPosition(m_StartingLocation);

	m_CurrentVelocity = 0;
	m_TransformComponent->SetRotation(Quaternion{});
}