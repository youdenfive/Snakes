#pragma once
#include <SFML/Graphics.hpp>
#include "wall.h"
#include "apple.h"
#include <deque>
#include <iostream>

// Скорость змейки
const float SPEED = CELL_SIZE;

class Snake {
public:
    Snake();
    Snake(sf::Vector2f startPosition, const std::vector<std::pair<std::string, std::string>>& control);
    Snake(const Snake& other);
    void setDefaultSnake(sf::Vector2f pos = sf::Vector2f(WIDTH / 2 * CELL_SIZE, HEIGHT / 2 * CELL_SIZE + CELL_SIZE));
    void grow();
    void handleInput(sf::RenderWindow& window, std::vector<std::pair<std::string, std::string>> control, sf::Keyboard::Scancode key);
    void handleInputTwo(sf::RenderWindow& window, sf::Keyboard::Scancode key);
    sf::Vector2f getHeadPosition() const;
    void draw(sf::RenderWindow& window);
    std::size_t getLength() const;
    void eatApple();
    void move();
    bool checkCollisionWithMyself();
    std::deque<sf::RectangleShape> getBody();
    sf::Vector2f getDirection();

    bool checkCollisionWithBody(const sf::Vector2f& position) const;
    void changeBotDirection(const sf::Vector2f& applePosition, const std::vector<Wall>& walls);
    bool checkCollisionWithWalls(const sf::Vector2f& position, const std::vector<Wall>& walls) const;

private:
    std::deque<sf::RectangleShape> body;
    sf::Vector2f direction;
    std::vector<std::pair<std::string, std::string>> _control;
};