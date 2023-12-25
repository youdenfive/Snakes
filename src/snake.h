#pragma once
#include <SFML/Graphics.hpp>
#include "wall.h"
#include "apple.h"
#include <deque>
#include <iostream>

// �������� ������
const float SPEED = CELL_SIZE;

class Snake {
public:
    Snake(sf::Vector2f startPosition, const std::vector<std::pair<std::string, std::string>>& control);
    Snake(const Snake& other);
    void grow();
    void handleInput(sf::RenderWindow& window, sf::Keyboard::Scancode key);
    sf::Vector2f getHeadPosition() const;
    void draw(sf::RenderWindow& window);
    std::size_t getLength() const;
    void eatApple();
    void move();
    bool checkCollisionWithMyself();
    std::deque<sf::RectangleShape> getBody();
    sf::Vector2f getDirection();
    bool checkCollisionWithBody(const sf::Vector2f& position) const;

private:
    std::deque<sf::RectangleShape> body;
    sf::Vector2f direction;
    std::vector<std::pair<std::string, std::string>> control;
};