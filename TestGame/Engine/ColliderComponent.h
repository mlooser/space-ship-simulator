#pragma once

#include <string>
#include <memory>

#include "reactphysics3d.h"

#include "Component.h"
#include "Event.h"
#include "EngineTypes.h"
#include "CollisionShapeDescription.h"

namespace MustacheEngine
{
	class TransformComponent;

	class ColliderComponent : public Component
	{
	private:
		CollisionShapeDescription m_ShapeDescription;

		rp3d::CollisionBody* m_CollisionBody;
		TransformComponent* m_Transform;

		std::shared_ptr<rp3d::ConvexShape> m_ColliderShape;
		rp3d::ProxyShape* m_ProxyShape;

		unsigned short m_CollisionLayer;
		unsigned short m_CollideWithLayers;

	public:

		ColliderComponent(const CollisionShapeDescription& shapeDescription)
			: m_ShapeDescription(shapeDescription)
			, m_CollisionLayer(0x1000)
			, m_CollideWithLayers(0xffff)
		{}

		ColliderComponent()
			: ColliderComponent({ CollisionShapeDescription::EShapeType::_Undefined,{0.0f,0.0f,0.0f} })
		{}

		~ColliderComponent();
		virtual void OnAttached(GameObject* owner) override;

		void UpdateCollision();

		rp3d::CollisionBody* GetCollisionBody() { return m_CollisionBody; }
		void SetCollisionBody(rp3d::CollisionBody* collisionBody);

		rp3d::Transform CalculateRPTransform() const;

		void SetShapeDescription(const CollisionShapeDescription& shapeDescription);

		void SetCollisionLayer(unsigned short collisionLayer);
		void SetCollideWithLayers(unsigned short layersMask);

	private:
		void RegenerateCollisionShape();
		void SetProxyShape(rp3d::ProxyShape* m_ProxyShape);
	};
}