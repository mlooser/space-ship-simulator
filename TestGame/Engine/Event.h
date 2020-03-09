#pragma once

#include <memory>
#include <vector>
#include <list>
#include <functional>
#include <algorithm>

#include "EventBindingInternal.h"

namespace MustacheEngine
{
	template<class ...TArg> class EventBinding;

	template<class ...TArg>
	class Event : public std::enable_shared_from_this<Event<TArg...>>
	{
	public:
		using EventBiningType = EventBinding<TArg...>;

	private:
		using Callback = std::function<void(TArg...)>;
		using EventBindingInternalElement = std::shared_ptr<EventBindingInternal<TArg...>>;

		bool m_IsInInvoke = false;
		std::vector<EventBindingInternalElement> m_Bindings;
		std::vector<EventBindingInternalElement> m_ToAdd;

	public:

		Event() = default;

		Event(const Event& ev) = delete;
		Event(Event&& ev) = delete;

		Event& operator=(const Event& ev) = delete;
		Event& operator=(Event&& ev) = delete;

		void Invoke(TArg... arg)
		{
			if (m_IsInInvoke)
				return;

			m_IsInInvoke = true;

			for (auto& it : m_Bindings)
			{
				it->Invoke(std::forward<TArg>(arg)...);
			}

			m_IsInInvoke = false;

			CleanupUnbinded();
			AddNewBindings();
		}


		EventBiningType Bind(const Callback& callback)
		{
			EventBindingInternalElement elem = std::make_shared<EventBindingInternal<TArg...>>(callback);
			if (m_IsInInvoke)
			{
				m_ToAdd.push_back(elem);
			}
			else
			{
				m_Bindings.push_back(elem);
			}
			EventBiningType ret(shared_from_this(), elem); //EventBiningType(shared_from_this(), elem);
			return ret;
		}

		void Unbind(EventBindingInternal<TArg...>* binding)
		{
			binding->UnBind();
		}

	private:

		void AddNewBindings()
		{
			if (m_ToAdd.size() == 0)
				return;

			m_Bindings.insert(m_Bindings.end(), m_ToAdd.begin(), m_ToAdd.end());
			m_ToAdd.clear();
		}

		void CleanupUnbinded()
		{
			m_Bindings.erase(
				std::remove_if(
					m_Bindings.begin(), m_Bindings.end(),
					[](EventBindingInternalElement& it) {return !it->IsBound(); }
			), m_Bindings.end());
		}
	};

	template<class ...TArg>
	class EventBinding
	{
	private:
		std::weak_ptr<EventBindingInternal<TArg...>> m_EBInternal;
		std::weak_ptr<Event<TArg...>> m_Owner;


	public:
		EventBinding()			
		{}

		EventBinding(std::weak_ptr<Event<TArg...>> owner, std::weak_ptr<EventBindingInternal<TArg...>> eb)
			: m_EBInternal(eb),
			m_Owner(owner)
		{
		}

		EventBinding(const EventBinding& eb) = delete;
		EventBinding& operator= (const EventBinding&) = delete;

		EventBinding(EventBinding&& eb)
		{
			m_EBInternal = std::move(eb.m_EBInternal);
			m_Owner = std::move(eb.m_Owner);
		}

		EventBinding& operator= (EventBinding&& eb)
		{
			m_EBInternal = std::move(eb.m_EBInternal);
			m_Owner = std::move(eb.m_Owner);
			return *this;
		}

		void Unbind()
		{
			auto owner = m_Owner.lock();

			if (!owner)
			{
				m_EBInternal.reset();
				return;
			}

			UnbindImpl(owner);
		}

		~EventBinding()
		{
			if (auto owner = m_Owner.lock())
			{
				UnbindImpl(owner);
			}
		}

	private:
		void UnbindImpl(std::shared_ptr<Event<TArg...>>& owner)
		{
			if (auto si = m_EBInternal.lock())
			{
				owner->Unbind(si.get());
				m_EBInternal.reset();
			}

			m_Owner.reset();
		}
	};

	template<class ...TArgs>
	using EventPtr = std::shared_ptr < Event<TArgs...>>;
}