#include <SFML/Graphics.hpp>
#include "gameMenu.h"
#include "singlepalyer.h"

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

int main()
{
    // Создаем окно игры.
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SNAKES", sf::Style::Fullscreen);

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
    std::vector<sf::String> name = {"START", "SETINGS", "ABOUT", "EXIT" };
    gameMenu gameMenu_(window, 950, 350, name, 150, 120);
    gameMenu_.alignTextMenu(1);
    
    // Инициализирует текст вложенных меню
    std::vector<sf::String> startGameName = { "SINGLEPLAYER", "MULTIPLAYER", "GO BACK" };
    std::vector<sf::String> setingsGameName = { "CONTROL", "THEME", "GO BACK"};
    std::vector<sf::String> aboutGameName = { "GO BACK" };

    // Отрисовывает окно.
    while (window.isOpen())
    {
        sf::Event event;

        // Обрабатывает события.
        while (window.pollEvent(event))
        {
            // Закрывает окно.
            if (event.type == sf::Event::Closed)
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
                            gameMenu_.pressButton(setingsGameName, 2, 2);
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
                    case 1:
                        switch (gameMenu_.getSelected()) {
                        case 0: singleplayer(window, gameMenu_); 
                            break;
                        case 1:break;
                        case 2:                            
                            gameMenu_.pressButton(name, 0);
                            break;
                        }
                        break;

                    // Меню настроек.
                    case 2:
                        switch (gameMenu_.getSelected()) {
                        case 0:break;
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