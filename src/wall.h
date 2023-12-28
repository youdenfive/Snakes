#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "settings.h"
#include <stdlib.h>

// Размер ячейки
const int CELL_SIZE = 30.0f;

class Wall {
public:
    Wall(sf::Vector2f position);
    const sf::Vector2f& getPosition() const;
    const sf::Sprite& getSprite() const;

private:
    sf::Vector2f position;
    sf::Sprite sprite;
};
