#include <SFML/Graphics.hpp>
#include "apple.h"

Apple::Apple() {
    respawn({});
}

sf::Vector2f Apple::getPosition() const {
    return position;
}

void Apple::respawn(const std::vector<Wall>& walls) {
    // √енерируем случайные координаты дл€ €блока в пределах пол€
    while (true) {
        position.x = static_cast<int>(rand() % (WIDTH - 2) + 1) * CELL_SIZE;
        position.y = static_cast<int>(rand() % (HEIGHT - 2) + 1) * CELL_SIZE;

        if (!isOnWall(walls)) {
            break;  // ¬ыход из цикла, если €блоко не находитс€ на стенах
        }
    }
}

bool Apple::isOnWall(const std::vector<Wall>& walls) const {
    // ѕровер€ем, находитс€ ли €блоко на стенах
    for (const auto& wall : walls) {
        if (static_cast<int>(position.x) == static_cast<int>(wall.getPosition().x)
            && static_cast<int>(position.y) == static_cast<int>(wall.getPosition().y)) {
            return true;
        }
    }
    return false;
}