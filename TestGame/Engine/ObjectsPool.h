#pragma once

#include <vector>
#include <stack>
#include <memory>
#include <functional>

#include "EngineMacros.h"

namespace MustacheEngine
{
	template<class TObject>
	class ObjectsPool
	{
	private:
		using Item = std::unique_ptr<TObject>;

		std::stack<Item, std::vector<Item>> m_Pool;
		std::function<TObject*(void)> m_FactoryFunction;

	public:

		ObjectsPool(const std::function<TObject*(void)> & factoryFunction)
			: m_FactoryFunction(factoryFunction)
		{

		}

		PREVENT_COPY_AND_MOVE(ObjectsPool);

		std::unique_ptr<TObject> GetObject()
		{
			if (m_Pool.size() == 0)
			{
				TObject* obj = m_FactoryFunction();
				return std::unique_ptr<TObject>(obj);
			}
			else
			{
				std::unique_ptr<TObject> ret(std::move(m_Pool.top()));
				m_Pool.pop();
				return std::move(ret);
			}
		}

		void GiveBackObject(std::unique_ptr<TObject> obj)
		{
			m_Pool.push(std::move(obj));
		}
	};
}
