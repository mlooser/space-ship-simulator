#pragma once

#include <memory>

#include "reactphysics3d.h"

#include "ResourceFactory.h"
#include "EngineMacros.h"
#include "CollisionShapeDescription.h"

namespace MustacheEngine
{
	class CollisionShapeFactory : public ResourceFactory<reactphysics3d::ConvexShape>
	{
	
	public:
		CollisionShapeFactory() = default;
		PREVENT_COPY_AND_MOVE(CollisionShapeFactory);

	public:
		std::shared_ptr<reactphysics3d::ConvexShape>& GetCollisionShape(const CollisionShapeDescription& shapeDescription);
	protected:
		virtual reactphysics3d::ConvexShape* LoadFromResourceImpl(const std::string& path) override;

	};
}