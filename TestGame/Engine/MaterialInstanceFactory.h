#pragma once

#include "ResourceFactory.h"
#include "EngineMacros.h"
#include "MaterialDefinitionFactory.h"
#include "MaterialInstance.h" //think about this more when I have some time

namespace MustacheEngine
{
	class MaterialInstance;	

	class MaterialInstanceFactory : public ResourceFactory<MaterialInstance>
	{
	private:
		MaterialDefinitionFactory m_MaterialDefinitionFactory;

	protected:
		virtual MaterialInstance* LoadFromResourceImpl(const std::string& path) override;
	
	};
}
