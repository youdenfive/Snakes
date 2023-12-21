#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

// Размер ячейки
const float CELL_SIZE = 30.0f;

class Wall {
public:
    Wall(sf::Vector2f position);
    const sf::Vector2f& getPosition() const;
    const sf::Sprite& getSprite() const;

private:
    sf::Vector2f position;
    sf::Sprite sprite;
};
