#pragma once

#include <unordered_map>
#include <string>

namespace MustacheEngine
{
	class ConfigReader
	{
	private:
		std::unordered_map<std::string, std::string> m_Config;

	public:
		void ReadConfig(const std::string& path);

		std::string GetString(const std::string& configKey) const;
		float GetFloat(const std::string& configKey, float defaultValue = 0.0f) const;
		bool GetBool(const std::string& configKey) const;

	private:
		void HandleCfgLine(const std::string& line);
	};
}
