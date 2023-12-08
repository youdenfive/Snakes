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
    do {
        position.x = static_cast<float>(rand() % (WIDTH - 2) + 1) * CELL_SIZE;
        position.y = static_cast<float>(rand() % (HEIGHT - 2) + 1) * CELL_SIZE;
    } while (isOnWall(walls));
}
bool Apple::isOnWall(const std::vector<Wall>& walls) const {
    // ѕровер€ем, находитс€ ли €блоко на стенах
    for (const auto& wall : walls) {
        if (position == wall.getPosition()) {
            return true;
        }
    }
    return false;
}