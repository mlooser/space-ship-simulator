#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "EngineMacros.h"

namespace MustacheEngine
{
	template<class TResource>
	class ResourceFactory
	{
	protected:
		std::unordered_map<std::string, std::shared_ptr<TResource>> m_LoadedResources;

		virtual TResource* LoadFromResourceImpl(const std::string& path) = 0;

	public:
		ResourceFactory() = default;

		PREVENT_COPY_AND_MOVE(ResourceFactory);
		
		std::shared_ptr<TResource>& LoadFromResource(const std::string& path)
		{
			auto& meshIt = m_LoadedResources.find(path);
			if (meshIt == m_LoadedResources.end())
			{
				TResource* mesh = LoadFromResourceImpl(path);
				m_LoadedResources.emplace(
					std::make_pair(
						path,
						std::move(std::shared_ptr<TResource>(mesh))
						));
			}
			return m_LoadedResources[path];
		}
	};
}
