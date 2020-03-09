#pragma once

#include <iostream>

namespace MustacheEngine
{
	class Logger
	{
	public:
		static Logger* GetInstance()
		{
			static Logger logger;
			return &logger;
		}

		template<class T>
		Logger* Log(T&& obj)
		{
			std::cout << obj;
			return this;
		}

		template<class T>
		Logger* LogLine(T&& obj)
		{
			return Log(std::forward<T>(obj))->Log("\n");
		}
	};
}
