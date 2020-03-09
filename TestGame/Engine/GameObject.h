#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <string>

#include "Component.h"
#include "EngineMacros.h"

namespace MustacheEngine
{
	class GameObject
	{
	private:
		std::vector<std::string> m_Tags;//I know, string are not the best
		std::vector<std::unique_ptr<Component>> m_AttachedComponents;
		bool m_IsActive = true;
	public:
		GameObject() = default;

		PREVENT_COPY_AND_MOVE(GameObject);

		void AddTag(const std::string& tag);
		bool HasTag(const std::string& tag) const;

		template<class TComponent, class ...Args>
		GameObject* AddComponent(Args&&... args)
		{
			TComponent* cmp = new TComponent(std::forward<Args>(args)...);
			cmp->OnAttached(this);
			cmp->SetActive(m_IsActive);
			m_AttachedComponents.emplace_back(cmp);
			return this;
		}

		template<class TComponent>
		TComponent* GetComponent()
		{
			for (auto& it = m_AttachedComponents.begin(); it != m_AttachedComponents.end(); ++it)
			{
				TComponent* ret = dynamic_cast<TComponent*>((*it).get());
				if (ret)
					return ret;
			}

			return nullptr;
		}

		void SetActive(bool active);
		bool IsActive() const { return m_IsActive; }

	};
}
