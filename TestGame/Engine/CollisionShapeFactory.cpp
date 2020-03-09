#include "CollisionShapeFactory.h"

#include <cstdio>
#include <stdexcept>

#include "CollisionShapeDescription.h"

using namespace MustacheEngine;
using namespace reactphysics3d;

std::shared_ptr<reactphysics3d::ConvexShape>& CollisionShapeFactory::GetCollisionShape(const CollisionShapeDescription& shapeDescription)
{
	return LoadFromResource(shapeDescription.GetDescriptionAsString());
}

reactphysics3d::ConvexShape* CollisionShapeFactory::LoadFromResourceImpl(const std::string& path)
{
	int type;
	float x, y, z;
	if (sscanf(path.c_str(), "t:%d;%f;%f;%f", &type, &x, &y, &z) != 4)
	{
		throw std::logic_error("Can't read shape definition " + path);
	}

	if (type != (int)CollisionShapeDescription::EShapeType::Box)
	{
		throw std::logic_error("Only Box is supported");
	}

	return new reactphysics3d::BoxShape(reactphysics3d::Vector3(x, y, z));
}