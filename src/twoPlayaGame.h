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

    Snake getPlayer1() {
        return player1;
    }

    Snake getPlayer2() {
        return player2;
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

    void draw(sf::RenderWindow& window, sf::Vector2i playersScore);

    void move();
    void handleInput(sf::RenderWindow& window, sf::Keyboard::Scancode key, Snake& snake);
    void initTextures();
    void drawSprites(sf::RenderWindow& window);
    void createSprites();
    void appleSkinChangerFunction2023AkaMultyplySomeSkinsTogether();
    void drawField(sf::RenderWindow& window);

private:
    Apple apple;  // ��������� ������ Apple

    sf::Font font;         // ����������� ������ ������
    sf::Text lengthText;   // ����������� ������ ������ ��� ����������� ���������� ������

    sf::Clock clock;        // ����������� ������ ����� ��� ��������� �������
    float updateInterval;   // ����������� ���� ��� �������� ��������� ����������
    sf::RectangleShape border;  // ����������� ������ �������������� ��� ������
    void initFont();
    std::vector<Wall> walls;
    sf::Clock appleRespawnTimer;
    sf::Texture fieldTexture;
    sf::Texture snake1Texture;
    sf::Texture appleTexture;
    sf::Texture snake2Texture;
    sf::Sprite appleSprite;
    std::deque<sf::Sprite> bodySprites;
    int score;

};