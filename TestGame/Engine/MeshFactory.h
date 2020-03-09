#pragma once

#include <memory>
#include <string>
#include <map>

#include "EngineMacros.h"
#include "ResourceFactory.h"
#include "Mesh.h"

namespace MustacheEngine
{
	class MeshFactory : public ResourceFactory<Mesh>
	{	
	protected:

		virtual Mesh* LoadFromResourceImpl(const std::string& path) override;
	};
}
