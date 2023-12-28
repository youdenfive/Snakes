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

void setDefaultSettings()
{
	std::vector<std::pair<std::string, std::string>> settings;

	settings.push_back(std::pair("Up1", "W"));
	settings.push_back(std::pair("Left1", "A"));
	settings.push_back(std::pair("Down1", "S"));
	settings.push_back(std::pair("Right1", "D"));
	settings.push_back(std::pair("Rounds", "1"));
	settings.push_back(std::pair("Bot", "OFF"));
	settings.push_back(std::pair("Nickname1", "Player1"));
	settings.push_back(std::pair("Nickname2", "Player2"));
	settings.push_back(std::pair("CellSize", "1"));
	settings.push_back(std::pair("Player1Color", "1"));
	settings.push_back(std::pair("Player2Color", "1"));
	settings.push_back(std::pair("AppleColor", "1"));
	settings.push_back(std::pair("BotColor", "1"));
	settings.push_back(std::pair("Up2", "Up"));
	settings.push_back(std::pair("Left2", "Left"));
	settings.push_back(std::pair("Down2", "Down"));
	settings.push_back(std::pair("Right2", "Right"));

	setSettings(settings);
}
