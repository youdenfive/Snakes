#include "serialize.h"

std::string serialize(std::vector<std::pair<std::string, std::string>> data)
{
	std::string result = "";

	for (auto elem : data) {
		result += elem.first + ":" + elem.second + "\n";
	}

	result = result.substr(0, result.rfind("\n"));

	return result;
}

std::vector<std::pair<std::string, std::string>> unserialize(std::string data)
{
	std::vector<std::pair<std::string, std::string>> result;
	//auto temp = data;

	while (!data.empty()) {
		size_t elemEnd = data.find("\n");
		std::string elem = data.substr(0, elemEnd);

		size_t delim = elem.find(":");
		std::string key = elem.substr(0, delim);
		std::string value = elem.substr(delim + 1);
		result.push_back(std::pair(key, value));
		data = data.substr(elemEnd + 1);
	}

	return result;
}
