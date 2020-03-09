#pragma once

namespace MustacheEngine
{
	class GameObject;

	class Component
	{
	protected:
		GameObject* m_Owner;
		bool m_IsActive = false;

	public:
		virtual void OnAttached(GameObject* owner)
		{
			m_Owner = owner;
		}

		void SetActive(bool activate) 
		{
			if (m_IsActive == activate)
				return;

			m_IsActive = activate;
			if (activate)
			{
				Activate();
			}
			else
			{
				Deactivate();
			}
		}		

		bool IsAtive() const { return m_IsActive; }

		virtual ~Component() {}

		GameObject* GetOwner() { return m_Owner; }

	protected:
		virtual void Activate() {}
		virtual void Deactivate() {}

		//virtual void OnObjectConstructed() {}
	};
}