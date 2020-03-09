#pragma once

namespace MustacheEngine
{
	template<class ...TArg>
	class EventBindingInternal
	{
	private:
		using Callback = std::function<void(TArg...)>;

		Callback m_Callback;
		bool m_IsBound;

	public:
		EventBindingInternal(const Callback& callback, bool isBound = true)
			:m_Callback(callback),
			m_IsBound(isBound)
		{

		}

		void Invoke(TArg... arg)
		{
			if (m_IsBound)
			{
				m_Callback(arg...);
			}
		}

		bool IsBound()const
		{
			return m_IsBound;
		}

		void UnBind()
		{
			m_IsBound = false;
		}
	};
}