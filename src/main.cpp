#include <SFML/Graphics.hpp>
#include "gameMenu.h"
#include "singlepalyer.h"
#include <iostream>
#include <string>

/**
*   �������������� �������� ���� � ����.
* 
*   @param sf::Text& mtext           �������������� �����.
*   @param float xpos                X-���� ������� ������.
*   @param float ypos                Y-���� ������� ������.
*   @param sf::String str            ���������� ������.
*   @param int size_font             ������ ������.
*   @param sf::Color menu_text_color ���� ������.
*   @param int bord                  ������� �������.
*   @param sf::Color border_color    ���� �������.
*
*   @return void
*/
void InitText(sf::Text& mtext, float xpos, float ypos, sf::String str, int size_font = 60,
    sf::Color menu_text_color = sf::Color::Black, int bord = 0, sf::Color border_color = sf::Color::White);

int main()
{
    // ������� ���� ����.
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SNAKES", sf::Style::Fullscreen);

    // ������ ������ ���������.
    window.setMouseCursorVisible(false);

    // ������� ����
    float width = sf::VideoMode::getDesktopMode().width;
    float height = sf::VideoMode::getDesktopMode().height;

    // ������������� ������ ��� ����.
    sf::RectangleShape background(sf::Vector2f(width, height));
    sf::Texture texture_window;
    if (!texture_window.loadFromFile("../designe/background_menu.jpg")) return 4;
    background.setTexture(&texture_window);

    // ������������� �����.
    sf::Font font;
    if (!font.loadFromFile("../designe/font/menuFont.ttf")) return 5;

    // ������������� ��������� � ������� ����.
    sf::Text titul;
    titul.setFont(font);
    InitText(titul, 580, 50, "SNAKES", 150, sf::Color(40,40,40), 3, sf::Color::Yellow);

    // �������������� ������� ����.
    std::vector<sf::String> name = {"START", "SETINGS", "ABOUT", "EXIT" };
    gameMenu gameMenu_(window, 950, 350, name, 150, 120);
    gameMenu_.alignTextMenu(1);
    
    // �������������� ����� ��������� ����
    std::vector<sf::String> startGameName = { "SINGLEPLAYER", "MULTIPLAYER", "GO BACK" };
    std::vector<sf::String> setingsGameName = { "CONTROL", "THEME", "GO BACK"};
    std::vector<sf::String> aboutGameName = { "GO BACK" };

    // �������������� ���� ����������.
    std::vector<sf::String> controlName = { "UP", "LEFT", "DOWN", "RIGHT", "APPLY", "GO BACK" };
    gameMenu controlMenu(window, 950, 350, controlName, 100, 70);
    controlMenu.alignTextMenu(2);

    sf::Text keyUp;
    sf::String keyName;
    Snake snake;
    singleplayer game(snake);

    // ������������ ����.
    while (window.isOpen())
    {
        sf::Event event;

        // ������������ �������.
        while (window.pollEvent(event))
        {
            // ��������� ����.
            if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
                window.close();

            // ������������ ������� ������.
            if (event.type == sf::Event::KeyReleased) {

                // ����� ����������� ������.
                if (event.key.code == sf::Keyboard::Up) {
                    gameMenu_.moveUp();
                }

                // ����� ����������� ������.
                if (event.key.code == sf::Keyboard::Down) {
                    gameMenu_.moveDown();
                }

                // ������������ ������� ������.
                if (event.key.code == sf::Keyboard::Enter) {
                    switch (gameMenu_.getMode()) {

                    // ������� ����.
                    case 0:
                        switch (gameMenu_.getSelected()) {
                        case 0:
                            gameMenu_.pressButton(startGameName, 1);
                            break;
                        case 1:
                            gameMenu_.pressButton(setingsGameName, 2);
                            break;
                        case 2:
                            gameMenu_.pressButton(aboutGameName, 3);
                            break;
                        case 3:
                            window.close();
                            break;
                        }
                        break;

                    // ���� ������ ����.
                    case 1:
                        switch (gameMenu_.getSelected()) {
                        case 0: 
                            game.startSingleplayer(window, gameMenu_);
                            break;
                        case 1:break;
                        case 2:                            
                            gameMenu_.pressButton(name, 0);
                            break;
                        }
                        break;

                    // ���� ��������.
                    case 2:
                        switch (gameMenu_.getSelected()) {
                        case 0: {
                            bool control = true;

                            while (control) {

                                while (window.pollEvent(event))
                                {
                                    // ��������� ����.
                                    if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
                                        window.close();

                                    // ������������ ������� ������.
                                    if (event.type == sf::Event::KeyReleased) {

                                        // ����� ����������� ������.
                                        if (event.key.code == sf::Keyboard::Up) {
                                            controlMenu.moveUp();
                                        }

                                        // ����� ����������� ������.
                                        if (event.key.code == sf::Keyboard::Down) {
                                            controlMenu.moveDown();
                                        }
                                        // ������������ ������� ������.
                                        //if (event.key.code == sf::Keyboard::Enter) {
                                            switch (controlMenu.getSelected()) {


                                            case 0:
                                                if (event.type == sf::Event::TextEntered)
                                                    keyName = "W";
                                                    InitText(keyUp, 950, 450, keyName, 15, sf::Color::Yellow, 3, sf::Color(40, 40, 40));
                                                  break;
                                            case 1:break;
                                            case 2:break;
                                            case 3:break;
                                                

                                                //if (event.key.code == sf::Keyboard::Enter) {
                                                    case 4:break;
                                                    case 5: 
                                                    if (event.key.code == sf::Keyboard::Enter) {
                                                        control = false;
                                                    }
                                                    break;
                                                //}
                                            }
                                    }
                                }
                                window.clear();
                                window.draw(background);
                                window.draw(titul);
                                controlMenu.draw();
                                window.draw(keyUp);
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

                    // ���� "about".
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

        // ���������.
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