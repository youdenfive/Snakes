#pragma once
#include <string>
#include <vector>

std::string serialize(std::vector< std::pair<std::string, std::string>> data);
std::vector<std::pair<std::string, std::string>> unserialize(std::string data);