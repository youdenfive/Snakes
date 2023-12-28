#include <SFML/Graphics.hpp>
#include "apple.h"

Apple::Apple() {
    respawn({});
}

sf::Vector2f Apple::getPosition() const {
    return position;
}

void Apple::respawn(const std::vector<Wall>& walls) {

    std::vector<std::pair<std::string, std::string>> settings;

    try {
        settings = getSettings();
    }
    catch (...) {
        setDefaultSettings();
        settings = getSettings();
    }

    position.x = static_cast<int>(rand() % (WIDTH - 2 * stoi(settings[8].second)) + stoi(settings[8].second)) * CELL_SIZE;
    position.y = static_cast<int>(rand() % (HEIGHT - 2 * stoi(settings[8].second)) + stoi(settings[8].second)) * CELL_SIZE;

    if (isOnWall(walls)) {
        respawn(walls);

    }

}

bool Apple::isOnWall(const std::vector<Wall>& walls) const {
    // Проверяем, находится ли яблоко на стенах
    for (const auto& wall : walls) {
        if (static_cast<int>(position.x) == static_cast<int>(wall.getPosition().x)
            && static_cast<int>(position.y) == static_cast<int>(wall.getPosition().y)) {
            return true;
        }
    }
    return false;
}