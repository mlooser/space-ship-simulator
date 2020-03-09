#include "ConfigReader.h"

#include <stdexcept>
#include <fstream>

using namespace MustacheEngine;
using namespace std;

void ConfigReader::ReadConfig(const std::string& path)
{
	string line;
	ifstream cfgFile(path);

	if (!cfgFile.is_open())
		throw runtime_error("Can't open file " + path);

	while (getline(cfgFile, line))
	{
		HandleCfgLine(line);
	}

	cfgFile.close();
}

string ConfigReader::GetString(const std::string& configKey) const
{
	const auto& it = m_Config.find(configKey);

	if (it == m_Config.cend())
	{
		return string{};
	}
	else
	{
		return it->second;
	}
}

float ConfigReader::GetFloat(const std::string& configKey, float defaultValue) const
{
	const auto& it = m_Config.find(configKey);

	if (it == m_Config.cend())		
	{
		return defaultValue;
	}
	else
	{ 		
		return stof(it->second);
	}		
}

bool ConfigReader::GetBool(const std::string& configKey) const
{
	return GetFloat(configKey) == 1.0f;
}

void ConfigReader::HandleCfgLine(const std::string& line)
{
	if (line.size() == 0 || line[0] == '#')//empty or comment
		return;

	auto splitIndex = line.find('=');
	if (splitIndex == string::npos)//incorrect format		
	{
		throw runtime_error("Incorrect cfg line format " + line);
	}

	string key = line.substr(0, splitIndex);
	string val = line.substr(splitIndex + 1);

	m_Config.emplace(std::make_pair(std::move(key), std::move(val)));
}