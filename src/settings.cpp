#include "settings.h"

std::vector<std::pair<std::string, std::string>> getSettings()
{
	std::ifstream inpF("settings.ini");

	if (!inpF.is_open()) {
		throw "settings is not exist";
		return std::vector<std::pair<std::string, std::string>>();
	}

	std::string settingsStr = "";

	while (!inpF.eof()) {
		std::string temp;
		inpF >> temp;
		settingsStr += temp + "\n";
	}

	auto settings = unserialize(settingsStr);

	return settings;
}

void setSettings(std::vector<std::pair<std::string, std::string>> data)
{
	std::ofstream outF("settings.ini");

	outF << serialize(data);

	outF.close();
}

std::vector<std::pair<std::string, std::string>> setDefaultSettings()
{
	std::vector<std::pair<std::string, std::string>> settings;

	settings.push_back(std::pair("Up", "W"));
	settings.push_back(std::pair("Left", "A"));
	settings.push_back(std::pair("Down", "S"));
	settings.push_back(std::pair("Right", "D"));
	settings.push_back(std::pair("Rounds", "1"));
	settings.push_back(std::pair("Bots", "0"));
	settings.push_back(std::pair("Nickname1", "Player1"));
	settings.push_back(std::pair("Nickname2", "Player2"));
	settings.push_back(std::pair("CellSize", "1"));

	return settings;
}
