#pragma once

#include <memory>
#include <vector>

#include "reactphysics3d.h"

#include "BaseSystem.h"
#include "CollisionInfo.h"
#include "Event.h"

namespace MustacheEngine
{
	class ColliderComponent;
	class CollisionShapeFactory;

	class PhysicsSystem : public BaseSystem, public rp3d::CollisionCallback
	{
	private:
		std::unique_ptr<rp3d::CollisionWorld> m_CollisionWorld;
		std::unique_ptr<CollisionShapeFactory> m_CollisionShapeFactory;
		std::vector<ColliderComponent*> m_Colliders;
		std::vector<CollisionInfo> m_OngoingCollisions;
		std::vector<CollisionInfo> m_NewCollisions;

	public:
		EventPtr<ColliderComponent*, ColliderComponent*> OnCollisionDetected;


	public:
		PhysicsSystem();

		virtual void Initialize(BaseGame* owner) override;
		virtual void Update() override;
		virtual void Shutdown() override;
		virtual void notifyContact(const rp3d::ContactPointInfo &contactPointInfo) override;

		rp3d::CollisionBody* AddCollider(ColliderComponent*);
		void RemoveCollider(ColliderComponent*);

		CollisionShapeFactory* GetCollisionShapeFactory();

	private:
		void CheckCollisions();
		void NotyfiAboutNewCollisions();
	};
}
