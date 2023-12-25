#pragma once
#include <SFML/Graphics.hpp>
#include "snake.h"
#include "gameMenu.h"
#include <deque>
#include <fstream>
#include "serialize.h"
#include <string>

class twoPlayaGame {
public:
    twoPlayaGame(std::string _playa1name = "ChostPlaya1", std::string _playa2name = "LowTabPlaya2");

    int startTwoPlayaGame(sf::RenderWindow& window);

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

    Snake player1;
    Snake player2;
    std::string playa1name;
    std::string playa2name;
    std::string collision;

protected:

    bool checkCollision();


    const sf::Font& getFont() const {
        return font;
    }

    const std::vector<Wall>& getWalls() const {
        return walls;
    }

    void draw(sf::RenderWindow& window);

    void move();
    void handleInput(sf::RenderWindow& window, sf::Keyboard::Scancode key, Snake& snake);
    int score();
    void initTextures();
    void drawSprites(sf::RenderWindow& window);
    void createSprites();
    void appleSkinChangerFunction2023AkaMultyplySomeSkinsTogether();

private:
    Apple apple;  // Добавляем объект Apple

    sf::Font font;         // Добавленный объект шрифта
    sf::Text lengthText;   // Добавленный объект текста для отображения количества клеток

    sf::Clock clock;        // Добавленный объект часов для измерения времени
    float updateInterval;   // Добавленный член для хранения интервала обновления
    sf::RectangleShape border;  // Добавленный объект прямоугольника для границ
    void initFont();
    std::vector<Wall> walls;
    sf::Clock appleRespawnTimer;
    sf::Texture snakeTexture;
    sf::Texture appleTexture;
    sf::Texture wallTexture;
    sf::Sprite appleSprite;
    std::deque<sf::Sprite> bodySprites;

};