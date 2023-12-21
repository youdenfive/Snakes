#pragma once
#include <SFML/Graphics.hpp>
#include "wall.h"

// Размер поля
const int WIDTH = 60;
const int HEIGHT = 30;

class Apple {
public:
    Apple();
    sf::Vector2f getPosition() const;
    void respawn(const std::vector<Wall>& walls);
    bool isOnWall(const std::vector<Wall>& walls) const;

private:
    sf::Vector2f position;
};