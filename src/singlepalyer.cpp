#include <SFML/Graphics.hpp>
#include "singlepalyer.h"

singleplayer::singleplayer(Snake _snake) : snake(_snake)
{     

    // Размеры окна
    float width = sf::VideoMode::getDesktopMode().width;
    float height = sf::VideoMode::getDesktopMode().height;

    // Инициализация прямоугольника границ
    border.setSize(sf::Vector2f(width - 2 * CELL_SIZE, height - 2 * CELL_SIZE));
    border.setFillColor(sf::Color::Transparent);  // Прозрачный цвет
    border.setOutlineColor(sf::Color::Yellow);      // Цвет границ
    border.setOutlineThickness(CELL_SIZE);              // Толщина границ

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

int singleplayer::startSingleplayer(sf::RenderWindow& window, gameMenu& menu)
{

    window.setFramerateLimit(10);

    // Инициализация генератора случайных чисел
    srand(static_cast<unsigned>(time(nullptr)));

    Snake snake;
    singleplayer game(snake);

    // Добавление стен
    game.map();
    game.initTextures();
    game.createSprites();
    // Установка начального интервала обновления
    game.updateSpeed();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }

        if (sf::Event::KeyReleased) {

            auto key = event.key.scancode;

            // После обработки событий ввода, обновляйте состояние кнопок
            game.handleInput(window, key);
        }

        // Обновление положения яблока, чтобы избежать столкновения с стенами
        game.getApple().respawn(game.getWalls());

        // Движение змейки
        game.move();

        updateSpeed(); // Вызываем для начальной установки интервала обновления

        // Инициализирует текст вложенных меню
        std::vector<sf::String> gameOverName = { "PLAY AGAIN", "BACK TO MENU" };
        gameMenu gameOverMenu(window, 950, 550, gameOverName, 100, 70);
        gameOverMenu.setColor(sf::Color::Red, sf::Color(220,220,220), sf::Color::Red);
        gameOverMenu.alignTextMenu(1);


        // Проверка на столкновение
        while (game.checkCollision()) {
            std::cout << "Game Over!" << std::endl;

            window.clear();

            // Отображение "Game Over" и длины змейки
            sf::Text gameOverText;
            gameOverText.setFont(game.getFont());
            gameOverText.setCharacterSize(40);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setString("Game Over!");
            gameOverText.setPosition(WIDTH * CELL_SIZE / 2 - 100, HEIGHT * CELL_SIZE / 2 - 20);

            sf::Text lengthText;
            lengthText.setFont(game.getFont());
            lengthText.setCharacterSize(20);
            lengthText.setFillColor(sf::Color::White);
            lengthText.setString("Length: " + std::to_string(game.score()));
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
                            game.startSingleplayer(window, menu);
                            return snake.getLength();

                        case 1:
                            std::vector<sf::String> name = { "START", "SETTINGS", "ABOUT", "EXIT" };
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
        game.draw(window);
        window.display();
    }

    return snake.getLength();
}

void singleplayer::initFont() {
    // Загружаем шрифт
    if (!font.loadFromFile("../designe/font/menuFont.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
    }
}

void singleplayer::updateSpeed() {
    // Пример: более короткий интервал для более длинной змейки

    updateInterval = std::max(0.1f, 1.0f - 0.05f * static_cast<float>(snake.getBody().size()));
}

bool singleplayer::checkCollision() {

    // Размеры окна
    float width = sf::VideoMode::getDesktopMode().width;
    float height = sf::VideoMode::getDesktopMode().height;

    // Проверка на столкновение с границами
    sf::Vector2f headPosition = snake.getHeadPosition();
    if (headPosition.x < CELL_SIZE || headPosition.x >= width - CELL_SIZE ||
        headPosition.y < CELL_SIZE || headPosition.y >= height - CELL_SIZE) {
        return true;
    }

    // Проверка столкновения с стенами
    for (const auto& wall : walls) {
        if (headPosition == wall.getPosition()) {
            return true;
        }
    }

    // Проверка на столкновение с самой собой
    return snake.checkCollisionWithMyself() ? true : false;
}

void singleplayer::map() {
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 5; i++) {
            addWall(sf::Vector2f(static_cast<int>(rand() % (WIDTH - 2) + 1) * CELL_SIZE, 
                static_cast<int>(rand() % (HEIGHT - 2) + 1) * CELL_SIZE));
        }
    }
}

void singleplayer::move() {

    // Получаем прошедшее время
    float elapsedTime = clock.getElapsedTime().asSeconds();

    // Копируем голову и добавляем новую голову в новую позицию
    sf::RectangleShape newHead = snake.getBody().front();
    newHead.move(snake.getDirection());

    sf::Vector2f headPosition = newHead.getPosition();

    // Проверка на поедание яблока
    if (headPosition == apple.getPosition()) {
        snake.eatApple();
        apple.respawn({});
        appleRespawnTimer.restart();
        createSprites();
    }
    else {
        // Удаляем последний сегмент змейки
        snake.move();
    }

    // Проверка времени с момента последнего респауна яблока
    if (appleRespawnTimer.getElapsedTime().asSeconds() >= 5) {
        // Респаун яблока и сброс таймера
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

void singleplayer::draw(sf::RenderWindow& window) {
    // Отрисовка стен и яблока
    drawSprites(window);

    // Отображение количества клеток змейки
    lengthText.setString("Length: " + std::to_string(snake.getBody().size()));
    lengthText.setPosition(10 + CELL_SIZE, 10 + CELL_SIZE);  // Позиция текста
    window.draw(lengthText);

    // Отрисовка заднего фона (границ)
    window.draw(border);
}


void singleplayer::drawSprites(sf::RenderWindow& window) {
    // Отрисовка стен
    for (const auto& wall : walls) {
        window.draw(wall.getSprite());
    }

    // Отрисовка каждого сегмента змейки с учетом поворота
    for (const auto& sprite : bodySprites) {
        window.draw(sprite);
    }

    // Отрисовка яблока
    window.draw(appleSprite);
}
void singleplayer::initTextures() {
    if (!snakeTexture.loadFromFile("../designe/snake.png")) {
        std::cerr << "Failed to load snake texture." << std::endl;
    }

    if (!appleTexture.loadFromFile("../designe/apple.png")) {
        std::cerr << "Failed to load apple texture." << std::endl;
    }

    if (!wallTexture.loadFromFile("../designe/wall.png")) {
        std::cerr << "Failed to load wall texture." << std::endl;
    }
}

void singleplayer::createSprites() {
    appleSprite.setTexture(appleTexture);
    appleSprite.setScale(CELL_SIZE / appleTexture.getSize().x, CELL_SIZE / appleTexture.getSize().y);
    appleSprite.setPosition(apple.getPosition());
    for (const auto& segment : snake.getBody()) {
        sf::Sprite segmentSprite(snakeTexture);
        segmentSprite.setScale(CELL_SIZE / snakeTexture.getSize().x, CELL_SIZE / snakeTexture.getSize().y);
        segmentSprite.setPosition(segment.getPosition());
        //segmentSprite.setRotation(rotation);  // Устанавливаем угол поворота
        bodySprites.push_back(segmentSprite);
    }

    for (const auto& wall : walls) {
        sf::Sprite wallSprite(wallTexture);
        wallSprite.setScale(CELL_SIZE / wallTexture.getSize().x, CELL_SIZE / wallTexture.getSize().y);
        wallSprite.setPosition(wall.getPosition());
        bodySprites.push_back(wallSprite);
    }
}

void singleplayer::handleInput(sf::RenderWindow& window, sf::Keyboard::Scancode key) 
{
    std::ifstream inpF("settings.ini");

    std::string settingsStr = "";
    std::string temp;

    while (!inpF.eof()) {
        inpF >> temp;
        settingsStr += temp + "\n";
    }

    auto settings = unserialize(settingsStr);

    snake.handleInput(window, settings, key);
}