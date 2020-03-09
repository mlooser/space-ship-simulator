#pragma once

#include <vector>

#include "EngineTypes.h"

namespace MustacheEngine
{
	class Mesh
	{
	public:
		std::vector<Vector3> Vertices;
		std::vector<Vector3> Normals;
		std::vector<Vector2> UVs;
	};
}
