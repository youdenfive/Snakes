#include <SFML/Graphics.hpp>
#include "apple.h"

Apple::Apple() {
    respawn({});
}

sf::Vector2f Apple::getPosition() const {
    return position;
}

void Apple::respawn(const std::vector<Wall>& walls) {
    // ���������� ��������� ���������� ��� ������ � �������� ����
    do {
        position.x = static_cast<float>(rand() % (WIDTH - 2) + 1) * CELL_SIZE;
        position.y = static_cast<float>(rand() % (HEIGHT - 2) + 1) * CELL_SIZE;
    } while (isOnWall(walls));
}
bool Apple::isOnWall(const std::vector<Wall>& walls) const {
    // ���������, ��������� �� ������ �� ������
    for (const auto& wall : walls) {
        if (position == wall.getPosition()) {
            return true;
        }
    }
    return false;
}