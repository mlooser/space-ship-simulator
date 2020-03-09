#include "GameObject.h"

#include <algorithm>

using namespace MustacheEngine;

void GameObject::SetActive(bool active)
{
	if (m_IsActive == active)
		return;//ignore

	m_IsActive = active;

	std::for_each(m_AttachedComponents.begin(), m_AttachedComponents.end(), [active](auto& cmp) {
		cmp->SetActive(active);
	});
}

bool GameObject::HasTag(const std::string& tag) const
{
	return std::find(m_Tags.cbegin(), m_Tags.cend(), tag) != m_Tags.cend();
}

void GameObject::AddTag(const std::string& tag)
{
	if (!HasTag(tag))
	{
		m_Tags.push_back(tag);
	}
}

