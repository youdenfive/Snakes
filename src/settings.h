#pragma once
#include <iostream>
#include <vector>
#include "serialize.h"
#include <fstream>

std::vector<std::pair<std::string, std::string>> getSettings();
void setSettings(std::vector<std::pair<std::string, std::string>> data);
std::vector<std::pair<std::string, std::string>> setDefaultSettings();