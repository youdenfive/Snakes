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
    Snake();
    void move();
    void grow();
    bool checkCollision();
    void handleInput(sf::RenderWindow& window);
    sf::Vector2f getHeadPosition() const;
    void draw(sf::RenderWindow& window);
    void updateSpeed();
    const sf::Font& getFont() const {
        return font;
    }
    std::size_t getLength() const;
    void addWall(sf::Vector2f position) {
        walls.emplace_back(position);
    }

    void drawWalls(sf::RenderWindow& window) {
        for (const auto& wall : walls) {
            wall.draw(window);
        }
    }
    bool checkWallCollision();
    void map();
    const std::vector<Wall>& getWalls() const {
        return walls;
    }
    Apple getApple() const {
        return apple;
    }

private:
    std::deque<sf::RectangleShape> body;
    sf::Vector2f direction;
    Apple apple;  // ��������� ������ Apple

    sf::Font font;         // ����������� ������ ������
    sf::Text lengthText;   // ����������� ������ ������ ��� ����������� ���������� ������

    sf::Clock clock;        // ����������� ������ ����� ��� ��������� �������
    float updateInterval;   // ����������� ���� ��� �������� ��������� ����������
    sf::RectangleShape border;  // ����������� ������ �������������� ��� ������
    void initFont();
    std::vector<Wall> walls;
};