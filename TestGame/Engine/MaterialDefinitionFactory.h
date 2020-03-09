#pragma once

#include "ResourceFactory.h"
#include "MaterialDefinition.h" //think about this more when I have some time

namespace MustacheEngine
{	
	class MaterialDefinitionFactory : public ResourceFactory<MaterialDefinition>
	{
	public:		
		MaterialDefinitionFactory() = default;

	protected:
		virtual MaterialDefinition* LoadFromResourceImpl(const std::string& path) override;
	};
}