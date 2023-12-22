#include <SFML/Graphics.hpp>
#include "gameMenu.h"
#include "singlepalyer.h"
#include <iostream>
#include <string>
#include <fstream>
#include "serialize.h"

/**
*   Инициализирует название игры в меню.
* 
*   @param sf::Text& mtext           Результирующий текст.
*   @param float xpos                X-овая позиция текста.
*   @param float ypos                Y-овая позиция текста.
*   @param sf::String str            Содержимое текста.
*   @param int size_font             Размер шрифта.
*   @param sf::Color menu_text_color Цвет текста.
*   @param int bord                  Толщина контура.
*   @param sf::Color border_color    Цвет контура.
*
*   @return void
*/
void InitText(sf::Text& mtext, float xpos, float ypos, sf::String str, int size_font = 60,
    sf::Color menu_text_color = sf::Color::Black, int bord = 0, sf::Color border_color = sf::Color::White);

void setControl(sf::Text& key, sf::String keyName, float ypos);

int gameSingleplayerStart(sf::RenderWindow& window, singleplayer game);

int main()
{
    // Создаем окно игры.
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SNAKES", sf::Style::Fullscreen);

    window.setFramerateLimit(60);

    // Делаем курсор невидимым.
    window.setMouseCursorVisible(false);

    // Размеры окна
    float width = sf::VideoMode::getDesktopMode().width;
    float height = sf::VideoMode::getDesktopMode().height;

    // Устанавливает задний фон меню.
    sf::RectangleShape background(sf::Vector2f(width, height));
    sf::Texture texture_window;
    if (!texture_window.loadFromFile("../designe/background_menu.jpg")) return 4;
    background.setTexture(&texture_window);

    // Устанавливает шрифт.
    sf::Font font;
    if (!font.loadFromFile("../designe/font/menuFont.ttf")) return 5;

    // Устанавливает заголовок в главном меню.
    sf::Text titul;
    titul.setFont(font);
    InitText(titul, 580, 50, "SNAKES", 150, sf::Color(40,40,40), 3, sf::Color::Yellow);

    // Инициализирует главное меню.
    std::vector<sf::String> name = {"START", "SETTINGS", "ABOUT", "EXIT" };
    gameMenu gameMenu_(window, 950, 350, name, 150, 120);
    gameMenu_.alignTextMenu(1);
    
    // Инициализирует текст вложенных меню
    std::vector<sf::String> startGameName = { "SINGLEPLAYER", "MULTIPLAYER", "GO BACK" };
    std::vector<sf::String> settingsGameName = { "CONTROL", "THEME", "GO BACK"};
    std::vector<sf::String> aboutGameName = { "GO BACK" };

    // Инициализирует меню управления.
    std::vector<sf::String> controlName = { "UP", "LEFT", "DOWN", "RIGHT", "APPLY", "GO BACK" };
    gameMenu controlMenu(window, 950, 350, controlName, 100, 70);
    controlMenu.alignTextMenu(2);

    // Инициализирует меню управления.
    std::vector<sf::String> singleplayerName = { "ROUNDS", "BOTS", "PLAY", "GO BACK" };
    gameMenu singleplayerMenu(window, 950, 350, singleplayerName, 100, 70);
    singleplayerMenu.alignTextMenu(2);

    sf::Text keyUp;
    sf::Text keyLeft;
    sf::Text keyDown;
    sf::Text keyRight;

    sf::Text keyRoundsCount;
    sf::Text keyBotsCount;

    sf::String keyName;
    Snake snake;
    singleplayer game(snake);

    // Отрисовывает окно.
    while (window.isOpen())
    {
        sf::Event event;

        std::ifstream inpF("settings.ini");

        std::string settingsStr = "";

        while (!inpF.eof()) {
            std::string temp;
            inpF >> temp;
            settingsStr += temp + "\n";
        }

        auto settings = unserialize(settingsStr);

        // Обрабатывает события.
        while (window.pollEvent(event))
        {
            // Закрывает окно.
            if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
                window.close();

            // Обрабатывает нажатие кнопки.
            if (event.type == sf::Event::KeyReleased) {

                // Выбор нижестоящей кнопки.
                if (event.key.code == sf::Keyboard::Up) {
                    gameMenu_.moveUp();
                }

                // Выбор нижестоящей кнопки.
                if (event.key.code == sf::Keyboard::Down) {
                    gameMenu_.moveDown();
                }

                // Обрабатывает нажатие кнопки.
                if (event.key.code == sf::Keyboard::Enter) {
                    switch (gameMenu_.getMode()) {

                    // Главное меню.
                    case 0:
                        switch (gameMenu_.getSelected()) {
                        case 0:
                            gameMenu_.pressButton(startGameName, 1);
                            break;
                        case 1:
                            gameMenu_.pressButton(settingsGameName, 2);
                            break;
                        case 2:
                            gameMenu_.pressButton(aboutGameName, 3);
                            break;
                        case 3:
                            window.close();
                            break;
                        }
                        break;

                    // Меню начала игры.
                    case 1: {

                        bool singleplayer = true;

                        // Обрабатывает нажатие кнопки.
                        sf::String roundsCount = settings[4].second;
                        sf::String botsCount = settings[5].second;

                        if (event.key.code == sf::Keyboard::Enter) {
                            switch (gameMenu_.getSelected()) {
                            case 0: {

                                while (singleplayer) {

                                    sf::Keyboard::Scancode code = sf::Keyboard::Scancode::Unknown;



                                    while (window.pollEvent(event))
                                    {
                                        // Закрывает окно.
                                        if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
                                            window.close();

                                        // Обрабатывает нажатие кнопки.
                                        if (event.type == sf::Event::KeyReleased) {

                                            // Выбор нижестоящей кнопки.
                                            if (event.key.code == sf::Keyboard::Up) {
                                                singleplayerMenu.moveUp();
                                            }

                                            // Выбор нижестоящей кнопки.
                                            if (event.key.code == sf::Keyboard::Down) {
                                                singleplayerMenu.moveDown();
                                            }



                                            switch (singleplayerMenu.getSelected()) {


                                            case 0:
                                                if (event.type == sf::Event::KeyReleased) {
                                                    code = event.key.scancode;
                                                    if (sf::Keyboard::getDescription(code) >= '0' && sf::Keyboard::getDescription(code) <= '9') {
                                                        roundsCount = sf::Keyboard::getDescription(code);
                                                    }
                                                }

                                                break;
                                            case 1:
                                                if (event.type == sf::Event::KeyReleased) {
                                                    code = event.key.scancode;
                                                    if (sf::Keyboard::getDescription(code) >= '0' && sf::Keyboard::getDescription(code) <= '9') {
                                                        botsCount = sf::Keyboard::getDescription(code);
                                                    }
                                                }

                                                break;
                                            case 2:
                                                if (event.key.code == sf::Keyboard::Enter) {
                                                    if (event.type == sf::Event::KeyReleased) {

                                                        std::string rounds = keyRoundsCount.getString();
                                                        std::string bots = keyBotsCount.getString();

                                                        //std::vector<std::pair<std::string, std::string>> controlSettings;

                                                        settings[4].second = rounds;
                                                        settings[5].second = bots;

                                                        std::ofstream outF("settings.ini");

                                                        outF << serialize(settings);

                                                        game.startSingleplayer(window, gameMenu_);
                                                    }
                                                }

                                                break;
                                            case 3:

                                                if (event.type == sf::Event::KeyReleased) {
                                                    singleplayer = false;
                                                }
                                                break;
                                            }

                                            keyRoundsCount.setFont(font);
                                            setControl(keyRoundsCount, roundsCount, 360);
                                            keyBotsCount.setFont(font);
                                            setControl(keyBotsCount, botsCount, 460);

                                            window.clear();
                                            window.draw(background);
                                            window.draw(titul);
                                            singleplayerMenu.draw();
                                            window.draw(keyRoundsCount);
                                            window.draw(keyBotsCount);
                                            window.display();
                                        }
                                    }
                                    break;
                                }
                            case 1:break;
                            case 2:
                                gameMenu_.pressButton(name, 0);
                                break;
                            }
                            }
                        }
                    }

                    // Меню настроек.
                    case 2:
                        switch (gameMenu_.getSelected()) {
                        case 0: {
                            bool control = true;

                            // Обрабатывает нажатие кнопки.
                            sf::String keyU = settings[0].second;
                            sf::String keyL = settings[1].second;
                            sf::String keyD = settings[2].second;
                            sf::String keyR = settings[3].second;
                            while (control) {

                                sf::Keyboard::Scancode code = sf::Keyboard::Scancode::Unknown;

                                while (window.pollEvent(event))
                                {
                                    // Закрывает окно.
                                    if (event.type == sf::Event::Closed)
                                        window.close();

                                    // Обрабатывает нажатие кнопки.
                                    if (event.type == sf::Event::KeyReleased) {

                                        // Выбор нижестоящей кнопки.
                                        if (event.key.code == sf::Keyboard::Up) {
                                            controlMenu.moveUp();
                                        }

                                        // Выбор нижестоящей кнопки.
                                        if (event.key.code == sf::Keyboard::Down) {
                                            controlMenu.moveDown();
                                        }

                                        switch (controlMenu.getSelected()) {


                                        case 0:
                                            if (event.type == sf::Event::KeyReleased) {
                                                code = event.key.scancode;
                                                if (sf::Keyboard::getDescription(code) != "Up" && sf::Keyboard::getDescription(code) != "Down") {
                                                    keyU = sf::Keyboard::getDescription(code);
                                                }
                                            }

                                                break;
                                        case 1:
                                            if (event.type == sf::Event::KeyReleased) {
                                                code = event.key.scancode;
                                                if (sf::Keyboard::getDescription(code) != "Up" && sf::Keyboard::getDescription(code) != "Down") {
                                                    keyL = sf::Keyboard::getDescription(code);
                                                }
                                                }

                                                break;
                                        case 2:
                                            if (event.type == sf::Event::KeyReleased) {
                                                code = event.key.scancode;
                                                if (sf::Keyboard::getDescription(code) != "Up" && sf::Keyboard::getDescription(code) != "Down") {
                                                    keyD = sf::Keyboard::getDescription(code);
                                                }
                                            }

                                                break;
                                        case 3:

                                            if (event.type == sf::Event::KeyReleased) {
                                                code = event.key.scancode;
                                                if (sf::Keyboard::getDescription(code) != "Up" && sf::Keyboard::getDescription(code) != "Down") {
                                                    keyR = sf::Keyboard::getDescription(code);
                                                }
                                            }

                                                break;

                                            case 4:
                                                if (event.key.code == sf::Keyboard::Enter) {

                                                    std::string Up    = keyUp.getString();
                                                    std::string Left  = keyLeft.getString();
                                                    std::string Down  = keyDown.getString();
                                                    std::string Right = keyRight.getString();

                                                    //std::vector<std::pair<std::string, std::string>> controlSettings;

                                                    settings[0].second = Up;
                                                    settings[1].second = Left;
                                                    settings[2].second = Down;
                                                    settings[3].second = Right;

                                                    std::ofstream outF("settings.ini");

                                                    outF << serialize(settings);
                                                }
                                                break;
                                            case 5:
                                                if (event.key.code == sf::Keyboard::Enter) {
                                                    control = false;
                                                }
                                                break;
                                            }
                                        }
                                    
                                }

                                keyLeft.setFont(font);
                                setControl(keyLeft, keyL, 460);
                                keyUp.setFont(font);
                                setControl(keyUp, keyU, 360);
                                keyDown.setFont(font);
                                setControl(keyDown, keyD, 560);
                                keyRight.setFont(font);
                                setControl(keyRight, keyR, 660);

                                window.clear();
                                window.draw(background);
                                window.draw(titul);
                                controlMenu.draw();
                                window.draw(keyUp);
                                window.draw(keyLeft);
                                window.draw(keyDown);
                                window.draw(keyRight);
                                window.display();
                            }
                            break;
                        }
                        case 1:break;
                        case 2:
                            gameMenu_.pressButton(name, 0);
                            break;
                        }
                        break;

                    // Меню "about".
                    case 3:
                        switch (gameMenu_.getSelected()) {
                        case 0:
                            gameMenu_.pressButton(name, 0);
                            break;
                        }
                        break;
                    }

                }
            }
        }

        // Отрисовка.
        window.clear();
        window.draw(background);
        window.draw(titul);
        gameMenu_.draw();
        window.display();
    }

    return 0;
}

void InitText(sf::Text& mtext, float xpos, float ypos, sf::String str, int size_font,
    sf::Color menu_text_color, int bord, sf::Color border_color)
{
    mtext.setCharacterSize(size_font);
    mtext.setPosition(xpos, ypos);
    mtext.setString(str);
    mtext.setFillColor(menu_text_color);
    mtext.setOutlineThickness(bord);
    mtext.setOutlineColor(border_color);
}

void setControl(sf::Text& key, sf::String keyName, float ypos)
{
    InitText(key, 1000, ypos, keyName, 55, sf::Color::Yellow, 3, sf::Color(40, 40, 40));
}

int gameSingleplayerStart(sf::RenderWindow& window, singleplayer game)
{
    int result = game.startSingleplayer(window);
    switch (result) {
    case 2 :
            gameSingleplayerStart(window, game);
    case 0 :
        return 1;
    }
    return 1;
}