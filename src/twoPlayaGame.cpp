#include <SFML/Graphics.hpp>
#include "twoPlayaGame.h"
#include <iostream>
#include <stdlib.h> // нужен для вызова функций rand(), srand()
#include <time.h> // нужен для вызова функции time()

twoPlayaGame::twoPlayaGame(std::string _playa1name, std::string _playa2name) : player1(sf::Vector2f(WIDTH / 3 * CELL_SIZE, HEIGHT / 2 * CELL_SIZE), { {"Up", "W"}, {"Left", "A"}, {"Down", "S"}, {"Right", "D"} }),
player2(sf::Vector2f(2 * WIDTH / 3 * CELL_SIZE, HEIGHT / 2 * CELL_SIZE), { {"Up", "Up"}, {"Left", "Left"}, {"Down", "Down"}, {"Right", "Right"} }), playa1name(_playa1name), playa2name(_playa2name)
{     
    collision = "";
    // Размеры окна
    float width = sf::VideoMode::getDesktopMode().width;
    float height = sf::VideoMode::getDesktopMode().height;

    // Инициализация прямоугольника границ
    border.setSize(sf::Vector2f(width - 2 * CELL_SIZE, height - 2 * CELL_SIZE));
    border.setFillColor(sf::Color::Transparent); // Прозрачный цвет
    border.setOutlineColor(sf::Color::Yellow);   // Цвет границ
    border.setOutlineThickness(CELL_SIZE);       // Толщина границ

    // Установка позиции прямоугольника границ, чтобы он оставался внутри экрана
    border.setPosition(CELL_SIZE, CELL_SIZE);

    // Загружаем шрифт
    if (!font.loadFromFile("../designe/font/menuFont.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
    }

    // Настройка параметров текста
    lengthText.setFont(font);
    lengthText.setCharacterSize(20);
    lengthText.setFillColor(sf::Color::White);

    initFont();  // Вызываем метод для инициализации шрифта
};

int twoPlayaGame::startTwoPlayaGame(sf::RenderWindow& window)
{

    window.setFramerateLimit(10);

    // Инициализация генератора случайных чисел
    srand(static_cast<unsigned>(time(nullptr)));

    initTextures();
    createSprites();

    // Установка начального интервала обновления
    updateSpeed();

    // Инициализирует текст вложенных меню
    std::vector<sf::String> gamePauseName = { "CONTINUE", "RESTART", "GO TO MENU"};
    gameMenu gamePauseMenu(window, 950, 550, gamePauseName, 100, 70);
    gamePauseMenu.setColor(sf::Color::Yellow, sf::Color(40, 40, 40), sf::Color::Yellow);
    gamePauseMenu.alignTextMenu(1);

    bool pause = false;

    std::vector<std::pair<std::string, std::string>> settings;

    try {
        settings = getSettings();
    }
    catch (...) {
        settings = setDefaultSettings();
    }

    auto roundsCount = std::stoi(settings[4].second);

    int score1 = 0;
    int score2 = 0;

    for (int roundNumber = 1; roundNumber <= roundsCount; ++roundNumber) {

        sf::Text round;
        round.setFont(font);
        round.setCharacterSize(30);
        round.setFillColor(sf::Color::White);
        round.setPosition(1560, 10 + CELL_SIZE);
        round.setString("Round: " + std::to_string(roundNumber) + '/' + std::to_string(roundsCount));

        player1.setDefaultSnake(sf::Vector2f(WIDTH / 3 * CELL_SIZE, HEIGHT / 2 * CELL_SIZE));
        player2.setDefaultSnake(sf::Vector2f(2 * WIDTH / 3 * CELL_SIZE, HEIGHT / 2 * CELL_SIZE));

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    auto key = event.key.scancode;
                    handleInput(window, key, player1);
                    handleInput(window, key, player2);
                }
            }

            if (event.key.code == sf::Keyboard::Escape) {
                pause = true;

                while (pause) {

                    while (window.pollEvent(event)) {

                        if (event.type == sf::Event::KeyReleased) {
                            // Выбор нижестоящей кнопки.
                            if (event.key.code == sf::Keyboard::Up) {
                                gamePauseMenu.moveUp();
                            }

                            // Выбор нижестоящей кнопки.
                            if (event.key.code == sf::Keyboard::Down) {
                                gamePauseMenu.moveDown();
                            }

                            if (event.key.code == sf::Keyboard::Enter) {
                                if (gamePauseMenu.getSelected() == 0) {
                                    pause = false;
                                    break;
                                }

                                switch (gamePauseMenu.getSelected()) {
                                case 1:
                                    return 2;
                                    break;

                                case 2:
                                    //std::vector<sf::String> name = { "START", "SETTINGS", "ABOUT", "EXIT" };
                                    //menu.pressButton(name, 0);
                                    return 0;
                                }
                            }
                        }
                    }

                    window.clear();
                    gamePauseMenu.draw();
                    window.display();
                }
            }

            // Обновление положения яблока, чтобы избежать столкновения с стенами
            getApple().respawn(getWalls());

            move();

            updateSpeed(); // Вызываем для начальной установки интервала обновления

            // Инициализирует текст вложенных меню
            std::vector<sf::String> gameOverName = { "PLAY AGAIN", "BACK TO MENU" };
            gameMenu gameOverMenu(window, 950, 550, gameOverName, 100, 70);
            gameOverMenu.setColor(sf::Color::Red, sf::Color(220, 220, 220), sf::Color::Red);
            gameOverMenu.alignTextMenu(1);

            if (checkCollision() && roundNumber < roundsCount) {
                score1 = score1 + getPlayer1().getBody().size() - 3;
                score2 = score2 + getPlayer2().getBody().size() - 3;
                break;
            }

            // Проверка на столкновение
            while (checkCollision()) {
                std::cout << "Game Over!" << std::endl;

                window.clear();

                // Отображение "Game Over" и длины змейки
                sf::Text gameOverText;
                gameOverText.setFont(getFont());
                gameOverText.setCharacterSize(40);
                gameOverText.setFillColor(sf::Color::Red);
                std::string winString = (collision != "nobody") ? collision + " win!" : collision + " wins..";
                gameOverText.setString(winString);
                gameOverText.setPosition(WIDTH * CELL_SIZE / 2 - 100, HEIGHT * CELL_SIZE / 2 - 20);

                sf::Text lengthText;
                lengthText.setFont(getFont());
                lengthText.setCharacterSize(20);
                lengthText.setFillColor(sf::Color::White);
                lengthText.setString("Length: " + std::to_string(score()));
                lengthText.setPosition(WIDTH * CELL_SIZE / 2 - 20, HEIGHT * CELL_SIZE / 2 + 40);

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
                                //startSingleplayer(window);
                                return 2;

                            case 1:
                                //std::vector<sf::String> name = { "START", "SETTINGS", "ABOUT", "EXIT" };
                                //menu.pressButton(name, 0);
                                return 0;
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
            window.draw(round);
            draw(window, sf::Vector2i(score1, score2));
            window.display();
        }
    }

    return score();
}

void twoPlayaGame::initFont() {
    // Загружаем шрифт
    if (!font.loadFromFile("../designe/font/menuFont.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
    }
}

void twoPlayaGame::updateSpeed() {
    // Пример: более короткий интервал для более длинной змейки

    updateInterval = std::max(0.1f, 1.0f - 0.05f * static_cast<float>(player1.getBody().size()));
}

bool twoPlayaGame::checkCollision() {
    bool result = false;

    // Размеры окна
    float width = sf::VideoMode::getDesktopMode().width;
    float height = sf::VideoMode::getDesktopMode().height;

    // Проверка на столкновение с границами
    sf::Vector2f headPosition1 = player1.getHeadPosition();
    if (headPosition1.x < CELL_SIZE || headPosition1.x >= width - CELL_SIZE ||
        headPosition1.y < CELL_SIZE || headPosition1.y >= height - CELL_SIZE) {
        collision = playa2name;
        result = true;
    }
    // Проверка на столкновение с границами
    sf::Vector2f headPosition2 = player2.getHeadPosition();
    if (headPosition2.x < CELL_SIZE || headPosition2.x >= width - CELL_SIZE ||
        headPosition2.y < CELL_SIZE || headPosition2.y >= height - CELL_SIZE) {
        collision = (!result) ? playa1name : "Nobody";
        result = true;
    }

    // Проверка на столкновение голов
    float distance = std::hypot(headPosition1.x - headPosition2.x, headPosition1.y - headPosition2.y);
    float minDistance = 0;  // Установите подходящее значение
    if (distance <= minDistance) {
        collision = "Nobody";
        result = true;
    }

    // Проверка на столкновение голов с телом
    if (player2.checkCollisionWithBody(headPosition1)) {
        collision = playa2name;
        result = true;
    }

    if (player1.checkCollisionWithBody(headPosition2)) {
        collision = playa1name;
        result = true;
    }
    // Проверка на с самими собой
    if (player1.checkCollisionWithMyself()) {
        collision = playa2name;
        result = true;
    }

    if (player2.checkCollisionWithMyself()) {
        collision = playa1name;
        result = true;
    }
    return result;
}

void twoPlayaGame::appleSkinChangerFunction2023AkaMultyplySomeSkinsTogether() {
    srand(time(NULL));
    int appleSkinNum = 1 + rand() % (4 - 1 + 1);
    if (!appleTexture.loadFromFile("../designe/apple" + std::to_string(appleSkinNum) + ".png")) {
        std::cerr << "Failed to load apple texture." << std::endl;
    }
}

void twoPlayaGame::move() {

    // Получаем прошедшее время
    float elapsedTime = clock.getElapsedTime().asSeconds();

    // Копируем голову и добавляем новую голову в новую позицию
    sf::RectangleShape newHead = player1.getBody().front();
    newHead.move(player1.getDirection());

    sf::Vector2f headPosition = newHead.getPosition();

    sf::RectangleShape newHead2 = player2.getBody().front();
    newHead2.move(player2.getDirection());

    sf::Vector2f headPosition2 = newHead2.getPosition();

    // Проверка на поедание яблока
    if (headPosition == apple.getPosition()) {
        player1.eatApple();
        appleSkinChangerFunction2023AkaMultyplySomeSkinsTogether();
        apple.respawn({});
        appleRespawnTimer.restart();
        createSprites();
    }
    else if (headPosition2 == apple.getPosition()) {
        player2.eatApple();
        appleSkinChangerFunction2023AkaMultyplySomeSkinsTogether();
        apple.respawn({});
        appleRespawnTimer.restart();
        createSprites();
    }
    else {
        player1.move();
        player2.move();
    }

    // Проверка времени с момента последнего респауна яблока
    if (appleRespawnTimer.getElapsedTime().asSeconds() >= 5) {
        // Респаун яблока и сброс таймера
        appleSkinChangerFunction2023AkaMultyplySomeSkinsTogether();
        apple.respawn(getWalls());
        appleRespawnTimer.restart();
    }

    // Проверяем, прошло ли достаточно времени для обновления
    if (elapsedTime > updateInterval) {
        clock.restart();

        // Обновляем скорость змейки в зависимости от длины
        updateSpeed();
    }
    // Очищаем вектор спрайтов перед обновлением
    bodySprites.clear();
    createSprites();
}

void twoPlayaGame::draw(sf::RenderWindow& window, sf::Vector2i playersScore) {
    // Отрисовка стен и яблока
    drawSprites(window);

    // Отображение количества клеток змейки
    lengthText.setString(playa1name + ":" + std::to_string(playersScore.x + getPlayer1().getBody().size() - 3));
    lengthText.setPosition(10 + CELL_SIZE, 10 + CELL_SIZE);  // Позиция текста
    window.draw(lengthText);

    // Отображение количества клеток змейки
    lengthText.setString(playa2name + ":" + std::to_string(playersScore.y + getPlayer2().getBody().size() - 3));
    lengthText.setPosition(10 + CELL_SIZE, 10 + 2*CELL_SIZE);  // Позиция текста
    window.draw(lengthText);

    // Отрисовка заднего фона (границ)
    window.draw(border);
}


void twoPlayaGame::drawSprites(sf::RenderWindow& window) {
    // Отрисовка каждого сегмента змейки с учетом поворота
    for (const auto& sprite : bodySprites) {
        window.draw(sprite);
    }

    // Отрисовка яблока
    window.draw(appleSprite);
}

void twoPlayaGame::initTextures() {
    srand(time(NULL));
    int num1 = 1 + rand() % 9;
    if (!snakeTexture.loadFromFile("../designe/snake" + std::to_string(num1) + ".png")) {
        std::cerr << "Failed to load snake1 texture." << std::endl;
    }
    
    appleSkinChangerFunction2023AkaMultyplySomeSkinsTogether();
    
    int num2 = (num1 + 1) % 10;
    if (!wallTexture.loadFromFile("../designe/snake" + std::to_string(num2) + ".png")) {
        std::cerr << "Failed to load snake2 texture." << std::endl;
    }
}

void twoPlayaGame::createSprites() {

    appleSprite.setTexture(appleTexture);
    appleSprite.setScale(CELL_SIZE / appleTexture.getSize().x, CELL_SIZE / appleTexture.getSize().y);
    appleSprite.setPosition(apple.getPosition());
    for (const auto& segment : player1.getBody()) {
        sf::Sprite segmentSprite(snakeTexture);
        segmentSprite.setScale(CELL_SIZE / snakeTexture.getSize().x, CELL_SIZE / snakeTexture.getSize().y);
        segmentSprite.setPosition(segment.getPosition());
        bodySprites.push_back(segmentSprite);
    }

    for (const auto& segment : player2.getBody()) {
        sf::Sprite segmentSprite(wallTexture);
        segmentSprite.setScale(CELL_SIZE / snakeTexture.getSize().x, CELL_SIZE / snakeTexture.getSize().y);
        segmentSprite.setPosition(segment.getPosition());
        bodySprites.push_back(segmentSprite);
    }
}

void twoPlayaGame::handleInput(sf::RenderWindow& window, sf::Keyboard::Scancode key, Snake& snake)
{
    snake.handleInputTwo(window, key);

}
int twoPlayaGame::score() {
    if (playa1name == collision) {
        return player1.getLength();
    }
    else if (playa2name == collision) {
        return player2.getLength();
    }
    else if (collision == "Nobody") {
        return 0;
    }
}