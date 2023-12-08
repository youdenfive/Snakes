#pragma once
#include <SFML/Graphics.hpp>

// Размер ячейки
const float CELL_SIZE = 30.0f;

class Wall {
public:
    Wall(sf::Vector2f position) : position(position) {
        shape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        shape.setPosition(position);
        shape.setFillColor(sf::Color::Blue);  // Цвет стен
    }

    const sf::Vector2f& getPosition() const {
        return position;
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(shape);
    }

private:
    sf::Vector2f position;
    sf::RectangleShape shape;
};