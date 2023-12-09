#pragma once
#include <SFML/Graphics.hpp>
#include "snake.h"
#include "gameMenu.h"
#include <deque>

class singleplayer {
public:
    singleplayer(Snake _snake);

    int startSingleplayer(sf::RenderWindow& window, gameMenu& menu);

    void updateSpeed();

    Apple getApple() const {
        return apple;
    }

    sf::Clock getClock() const {
        return clock;
    }

    float getUpdateInterval() const {
        return updateInterval;
    }

protected:

    bool checkCollision();


    const sf::Font& getFont() const {
        return font;
    }

    void addWall(sf::Vector2f position) {
        walls.emplace_back(position);
    }

    void drawWalls(sf::RenderWindow& window) {
        for (const auto& wall : walls) {
            wall.draw(window);
        }
    }

    void map();
    const std::vector<Wall>& getWalls() const {
        return walls;
    }

    void draw(sf::RenderWindow& window);

    void move();
    void handleInput(sf::RenderWindow& window) { snake.handleInput(window); };
    int score() { return snake.getLength(); };

private:
    Snake snake;
    Apple apple;  // ��������� ������ Apple

    sf::Font font;         // ����������� ������ ������
    sf::Text lengthText;   // ����������� ������ ������ ��� ����������� ���������� ������

    sf::Clock clock;        // ����������� ������ ����� ��� ��������� �������
    float updateInterval;   // ����������� ���� ��� �������� ��������� ����������
    sf::RectangleShape border;  // ����������� ������ �������������� ��� ������
    void initFont();
    std::vector<Wall> walls;
};