#include "settings.h"

std::vector<std::pair<std::string, std::string>> getSettings()
{
	std::ifstream inpF("settings.ini");

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
