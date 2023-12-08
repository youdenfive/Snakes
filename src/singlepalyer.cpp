#include <SFML/Graphics.hpp>
#include "snake.h"
#include "singlepalyer.h"
#include "gameMenu.h"

int singleplayer(sf::RenderWindow& window, gameMenu& menu) 
{

    window.setFramerateLimit(10);

    // Инициализация генератора случайных чисел
    srand(static_cast<unsigned>(time(nullptr)));

    Snake snake;

    // Добавление стен
    snake.map();

    // Установка начального интервала обновления
    snake.updateSpeed();
    snake.draw(window);  // Рисуем границы перед основным циклом

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }

        // После обработки событий ввода, обновляйте состояние кнопок
        snake.handleInput(window);

        // Обновление положения яблока, чтобы избежать столкновения с стенами
        snake.getApple().respawn(snake.getWalls());

        // Движение змейки
        snake.move();

        // Инициализирует текст вложенных меню
        std::vector<sf::String> gameOverName = { "PLAY AGAIN", "BACK TO MENU" };
        gameMenu gameOverMenu(window, 950, 550, gameOverName, 100, 70);
        gameOverMenu.setColor(sf::Color::Red, sf::Color(220,220,220), sf::Color::Red);
        gameOverMenu.alignTextMenu(1);

        // Проверка на столкновение
        while (snake.checkCollision()) {
            std::cout << "Game Over!" << std::endl;

            window.clear();

            // Отображение "Game Over" и длины змейки
            sf::Text gameOverText;
            gameOverText.setFont(snake.getFont());
            gameOverText.setCharacterSize(40);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setString("Game Over!");
            gameOverText.setPosition(WIDTH * CELL_SIZE / 2 - 100, HEIGHT * CELL_SIZE / 2 - 20);

            sf::Text lengthText;
            lengthText.setFont(snake.getFont());
            lengthText.setCharacterSize(20);
            lengthText.setFillColor(sf::Color::White);
            lengthText.setString("Length: " + std::to_string(snake.getLength()));
            lengthText.setPosition(WIDTH * CELL_SIZE / 2 - 20, HEIGHT * CELL_SIZE / 2 + 40);

            //sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
                    window.close();
                    return EXIT_SUCCESS;
                }

                // Обрабатывает нажатие кнопки.
                if (event.type == sf::Event::KeyReleased) {

                    // Выбор нижестоящей кнопки.
                    if (event.key.code == sf::Keyboard::Up) {
                        gameOverMenu.moveUp();
                    }

                    // Выбор нижестоящей кнопки.
                    if (event.key.code == sf::Keyboard::Down) {
                        gameOverMenu.moveDown();
                    }

                    if (event.key.code == sf::Keyboard::Enter) {
                        switch (gameOverMenu.getSelected()) {
                        case 0:
                            singleplayer(window, menu);
                            return snake.getLength();

                        case 1:
                            std::vector<sf::String> name = { "START", "SETINGS", "ABOUT", "EXIT" };
                            menu.pressButton(name, 0);
                            return snake.getLength();
                        }
                    }
                }
            }

            window.draw(gameOverText);
            window.draw(lengthText);
            gameOverMenu.draw();

            window.display();
        }

        // Отрисовка
        window.clear();
        snake.draw(window);
        window.display();
    }

    return snake.getLength();
}
