#pragma once
#include <SFML/Graphics.hpp>
#include "snake.h"
#include "gameMenu.h"
#include <deque>
#include <fstream>
#include "serialize.h"

class singleplayer {
public:
    singleplayer(Snake _snake);

    int startSingleplayer(sf::RenderWindow& window);

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
            window.draw(wall.getSprite());
        }
    }

    void map();
    const std::vector<Wall>& getWalls() const {
        return walls;
    }

    void draw(sf::RenderWindow& window, int& score);

    void move();
    void handleInput(sf::RenderWindow& window, sf::Keyboard::Scancode key);
    int score() { return snake.getLength(); };
    void initTextures();
    void drawSprites(sf::RenderWindow& window);
    void createSprites();
    Snake getSnake();

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
    sf::Clock appleRespawnTimer;
    sf::Texture snakeTexture;
    sf::Texture appleTexture;
    sf::Texture wallTexture;
    sf::Sprite appleSprite;
    std::deque<sf::Sprite> bodySprites;
};