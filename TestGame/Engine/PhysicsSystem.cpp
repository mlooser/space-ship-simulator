#include "PhysicsSystem.h"

#include <algorithm>

#include "ColliderComponent.h"
#include "Logger.h"
#include "CollisionShapeFactory.h"

using namespace MustacheEngine;
using namespace reactphysics3d;

PhysicsSystem::PhysicsSystem()
	:OnCollisionDetected(std::make_shared<Event<ColliderComponent*, ColliderComponent*>>())
{

}

void PhysicsSystem::Initialize(BaseGame* owner)
{
	BaseSystem::Initialize(owner);

	m_CollisionWorld = std::make_unique<rp3d::CollisionWorld>();
	m_CollisionShapeFactory = std::make_unique<CollisionShapeFactory>();
}

void PhysicsSystem::Shutdown()
{
	BaseSystem::Shutdown();

	std::for_each(m_Colliders.begin(), m_Colliders.end(), [](ColliderComponent* cc) {cc->SetCollisionBody(nullptr); });
	m_Colliders.clear();
	m_CollisionWorld.release();
}

CollisionBody* PhysicsSystem::AddCollider(ColliderComponent* collider)
{
	Transform transform = collider->CalculateRPTransform();
	CollisionBody* collisionBody = m_CollisionWorld->createCollisionBody(transform);
	m_Colliders.push_back(collider);
	collider->SetCollisionBody(collisionBody);
	return collisionBody;
}

void PhysicsSystem::Update()
{
	BaseSystem::Update();

	for (ColliderComponent* collider : m_Colliders)
	{
		collider->UpdateCollision();
	}

	CheckCollisions();
	NotyfiAboutNewCollisions();
}

void PhysicsSystem::CheckCollisions()
{
	std::for_each
	(
		m_OngoingCollisions.begin(),
		m_OngoingCollisions.end(),
		[](CollisionInfo& ci) {ci.IsOngoing = false; }
	);

	m_CollisionWorld->testCollision(this);

	m_OngoingCollisions.erase
	(
		std::remove_if
		(
			m_OngoingCollisions.begin(),
			m_OngoingCollisions.end(),
			[](CollisionInfo& ci) {return !ci.IsOngoing; }
		),
		m_OngoingCollisions.end()
	);
}

void PhysicsSystem::notifyContact(const rp3d::ContactPointInfo &contactPointInfo)
{
	ColliderComponent* firstCollider = (ColliderComponent*)contactPointInfo.shape1->getUserData();
	ColliderComponent* secondCollider = (ColliderComponent*)contactPointInfo.shape2->getUserData();

	if (!(firstCollider->IsAtive() && secondCollider->IsAtive()))
		return;

	auto it = std::find_if
	(
		m_OngoingCollisions.begin(),
		m_OngoingCollisions.end(),
		[firstCollider, secondCollider](CollisionInfo& ci) {return ci.IfMatch(firstCollider, secondCollider); }
	);

	if (it == m_OngoingCollisions.end())//new collision detected
	{
		//Logger::GetInstance()->LogLine("New collision detected!");
		m_NewCollisions.emplace_back(CollisionInfo{ firstCollider,secondCollider,true });
	}
	else
	{
		it->IsOngoing = true;
	}
}

void PhysicsSystem::NotyfiAboutNewCollisions()
{
	std::for_each
	(
		m_NewCollisions.begin(),
		m_NewCollisions.end(),
		[this](CollisionInfo& ci)
	{
		this->OnCollisionDetected->Invoke(ci.First, ci.Second);
		this->m_OngoingCollisions.push_back(ci);
	}
	);

	m_NewCollisions.clear();
}


void PhysicsSystem::RemoveCollider(ColliderComponent* collider)
{
	m_Colliders.erase(
		std::remove(m_Colliders.begin(), m_Colliders.end(), collider),
		m_Colliders.end());

	m_CollisionWorld->destroyCollisionBody(collider->GetCollisionBody());
	collider->SetCollisionBody(nullptr);
}

CollisionShapeFactory* PhysicsSystem::GetCollisionShapeFactory()
{
	return m_CollisionShapeFactory.get();
}