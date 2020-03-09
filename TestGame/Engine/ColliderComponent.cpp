#include "ColliderComponent.h"

#include "PhysicsSystem.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "EngineTypes.h"
#include "BaseGame.h"
#include "CollisionShapeFactory.h"

using namespace MustacheEngine;

void ColliderComponent::OnAttached(GameObject* owner)
{
	Component::OnAttached(owner);

	m_Transform = owner->GetComponent<TransformComponent>();

	RegenerateCollisionShape();
	BaseGame::GetInstance()->GetSystem<PhysicsSystem>()->AddCollider(this);
}

void ColliderComponent::UpdateCollision()
{
	if (m_CollisionBody)
	{
		m_CollisionBody->setTransform(CalculateRPTransform());
	}
}

rp3d::Transform ColliderComponent::CalculateRPTransform() const
{
	const Vector3& position = m_Transform->GetPosition();
	const Quaternion& rotation = m_Transform->GetRotation();

	rp3d::Vector3 posistionRP(position.x, position.y, position.z);//TODO try some smart cast/union
	rp3d::Quaternion rotationRP(rotation.x, rotation.y, rotation.z, rotation.w);//TODO try some smart cast/union
	rp3d::Transform retTransform(posistionRP, rotationRP);

	return retTransform;
}

void ColliderComponent::SetShapeDescription(const CollisionShapeDescription& shapeDescription)
{
	m_ShapeDescription = shapeDescription;
	RegenerateCollisionShape();
}
void ColliderComponent::SetCollisionBody(rp3d::CollisionBody* collisionBody)
{
	reactphysics3d::Transform tr = reactphysics3d::Transform::identity();
	m_CollisionBody = collisionBody;

	if (m_ColliderShape.get())
	{
		rp3d::ProxyShape* proxyShape = m_CollisionBody->addCollisionShape(m_ColliderShape.get(), tr);
		SetProxyShape(proxyShape);
	}
}

ColliderComponent::~ColliderComponent()
{
	if (!m_CollisionBody)
	{
		if (m_ProxyShape)
		{
			m_CollisionBody->removeCollisionShape(m_ProxyShape);
		}
		BaseGame::GetInstance()->GetSystem<PhysicsSystem>()->RemoveCollider(this);
	}
}

void ColliderComponent::RegenerateCollisionShape()
{
	if (m_ShapeDescription.IsValid())
	{
		if (m_CollisionBody != nullptr && m_ProxyShape != nullptr)
		{
			m_CollisionBody->removeCollisionShape(m_ProxyShape);
		}

		PhysicsSystem* physSystem = BaseGame::GetInstance()->GetSystem<PhysicsSystem>();
		m_ColliderShape = physSystem->GetCollisionShapeFactory()->GetCollisionShape(m_ShapeDescription);

		if (m_CollisionBody != nullptr)
		{
			rp3d::ProxyShape* proxyShape = m_CollisionBody->addCollisionShape(m_ColliderShape.get(), reactphysics3d::Transform::identity());
			SetProxyShape(proxyShape);
		}

	}
}

void ColliderComponent::SetCollisionLayer(unsigned short collisionLayer)
{ 
	m_CollisionLayer = collisionLayer; 
	if (m_ProxyShape)
	{
		m_ProxyShape->setCollisionCategoryBits(m_CollisionLayer);
	}
}

void ColliderComponent::SetCollideWithLayers(unsigned short layersMask)
{ 
	m_CollideWithLayers = layersMask; 
	if (m_ProxyShape)
	{
		m_ProxyShape->setCollideWithMaskBits(m_CollideWithLayers);
	}
}

void ColliderComponent::SetProxyShape(rp3d::ProxyShape* proxyShape)
{
	m_ProxyShape = proxyShape;
	m_ProxyShape->setUserData(this);
	m_ProxyShape->setCollisionCategoryBits(m_CollisionLayer);
	m_ProxyShape->setCollideWithMaskBits(m_CollideWithLayers);
}