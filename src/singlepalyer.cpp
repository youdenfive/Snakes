#include <SFML/Graphics.hpp>
#include "singlepalyer.h"
#include <string>

singleplayer::singleplayer(Snake _snake) : snake(_snake)
{     
    std::vector<std::pair<std::string, std::string>> settings;

    try {
        settings = getSettings();
    }
    catch (...) {
        setDefaultSettings();
        settings = getSettings();
    }

    // Размеры окна
    float width = sf::VideoMode::getDesktopMode().width;
    float height = sf::VideoMode::getDesktopMode().height;

    // Инициализация прямоугольника границ
    border.setSize(sf::Vector2f(width - 2 * stoi(settings[8].second) * CELL_SIZE, height - 2 * stoi(settings[8].second) * CELL_SIZE));
    border.setFillColor(sf::Color::Transparent);  // Прозрачный цвет
    border.setOutlineColor(sf::Color::Yellow);      // Цвет границ
    border.setOutlineThickness(stoi(settings[8].second) * CELL_SIZE);              // Толщина границ

    // Установка позиции прямоугольника границ, чтобы он оставался внутри экрана
    border.setPosition(stoi(settings[8].second) * CELL_SIZE, stoi(settings[8].second) * CELL_SIZE);

    // Загружаем шрифт
    if (!font.loadFromFile("../designe/font/menuFont.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
    }

    // Настройка параметров текста
    lengthText.setFont(font);
    lengthText.setCharacterSize(20);
    lengthText.setFillColor(sf::Color::White);

    initFont();  // Вызываем метод для инициализации шрифта
    collision = "";
    _score = 0;
};

int singleplayer::startSingleplayer(sf::RenderWindow& window)
{

    window.setFramerateLimit(10);

    std::vector<std::pair<std::string, std::string>> settings;

    try {
        settings = getSettings();
    }
    catch (...) {
        setDefaultSettings();
        settings = getSettings();
    }

    if (settings[5].second == "ON") {
        botIsOn = true;
    }
    else {
        botIsOn = false;
    }

    int score = 0;
    int botScore = 0;

    sf::Text nickname;
    nickname.setFont(font);
    nickname.setCharacterSize(30);
    nickname.setFillColor(sf::Color::White);
    nickname.setPosition(860, 10 + stoi(settings[8].second) * CELL_SIZE);
    nickname.setString(settings[6].second);

    // Инициализирует текст вложенных меню
    std::vector<sf::String> gameOverName = { "PLAY AGAIN", "BACK TO MENU" };
    gameMenu gameOverMenu(window, 950, 550, gameOverName, 100, 70);
    gameOverMenu.setColor(sf::Color::Red, sf::Color(220, 220, 220), sf::Color::Red);
    gameOverMenu.alignTextMenu(1);


    sf::Event event;

    // Инициализация генератора случайных чисел
    srand(static_cast<unsigned>(time(nullptr)));
    int roundsCount = stoi(settings[4].second);

    for (int roundNumber = 1; roundNumber <= roundsCount; ++roundNumber) {
        collision = "";
        sf::Text round;
        round.setFont(font);
        round.setCharacterSize(30);
        round.setFillColor(sf::Color::White);
        round.setPosition(1560, 10 + stoi(settings[8].second) * CELL_SIZE);
        round.setString("Round: " + std::to_string(roundNumber) + '/' + std::to_string(roundsCount));

        deleteWalls();
        snake.setDefaultSnake();
        if (botIsOn) {
            bot.setDefaultSnake(sf::Vector2f(2 * WIDTH / 3 * CELL_SIZE, HEIGHT / 2 * CELL_SIZE));
        }
        // Добавление стен
        map();
        initTextures();
        createSprites();
        // Установка начального интервала обновления
        updateSpeed();

        // Инициализирует текст вложенных меню
        std::vector<sf::String> gamePauseName = { "CONTINUE", "RESTART", "GO TO MENU" };
        gameMenu gamePauseMenu(window, 950, 550, gamePauseName, 100, 70);
        gamePauseMenu.setColor(sf::Color::Yellow, sf::Color(40, 40, 40), sf::Color::Yellow);
        gamePauseMenu.alignTextMenu(1);

        bool pause = false;

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            // 
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

            if (sf::Event::KeyReleased) {

                auto key = event.key.scancode;

                // После обработки событий ввода, обновляйте состояние кнопок
                handleInput(window, key);
            }

            // Обновление положения яблока, чтобы избежать столкновения с стенами
            getApple().respawn(getWalls());

            if (botIsOn) {
                bot.changeBotDirection(apple.getPosition(), walls);
            }

            // Движение змейки
            move();

            updateSpeed(); // Вызываем для начальной установки интервала обновления


            if (checkCollision()) {
                score = score + getSnake().getBody().size() - 3;
                botScore = botScore + bot.getLength() - 3;
                if (collision == "playa" && botIsOn) {
                    score += 5;
                }
                else if (collision == "bot" && botIsOn) {
                    botScore += 5;
                }
                else if (collision == "playa kill" && botIsOn) {
                    score += 5;
                }
                else if (collision == "bot kill" && botIsOn) {
                    botScore += 5;
                }
                break;
            }

            sf::RectangleShape player;
            sf::Texture playerTexture;
            std::string playerTexturePath = "../designe/snake" + settings[9].second + ".png";
            if (!playerTexture.loadFromFile(playerTexturePath)) return -1;
            player.setSize(sf::Vector2f(30, 30));
            player.setPosition(sf::Vector2f(820, 10 + stoi(settings[8].second) * CELL_SIZE));
            player.setTexture(&playerTexture);

            // Отрисовка
            window.clear();
            draw(window, score);
            window.draw(player);
            window.draw(nickname);
            window.draw(round);

            window.display();
        }
    }


    // Проверка на столкновение
    while (true) {

        window.clear();
        int winScore = (score > botScore) ? score : botScore;
        std::string winner = (score == winScore) ? ((botScore == winScore) ? "nobody" : settings[6].second) : "Bot";
        winner = (botIsOn) ? winner + " win!" : "Game Over";
        _score = winScore;

        // Отображение "Game Over" и длины змейки
        sf::Text gameOverText;
        gameOverText.setFont(getFont());
        gameOverText.setCharacterSize(40);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setString(winner);
        gameOverText.setPosition(WIDTH * CELL_SIZE / 2 - 100, HEIGHT * CELL_SIZE / 2 - 20);

        sf::Text lengthText;
        lengthText.setFont(getFont());
        lengthText.setCharacterSize(20);
        lengthText.setFillColor(sf::Color::White);
        lengthText.setString("Score: " + std::to_string(winScore));
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
    return _score;
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
    bool result = false;
    // Размеры окна
    float width = sf::VideoMode::getDesktopMode().width;
    float height = sf::VideoMode::getDesktopMode().height;

    std::vector<std::pair<std::string, std::string>> settings;

    try {
        settings = getSettings();
    }
    catch (...) {
        setDefaultSettings();
        settings = getSettings();
    }

    // Проверка на столкновение с границами
    sf::Vector2f headPosition1 = snake.getHeadPosition();
    if (headPosition1.x < stoi(settings[8].second) * CELL_SIZE || headPosition1.x >= width - stoi(settings[8].second) * CELL_SIZE ||
        headPosition1.y < stoi(settings[8].second) * CELL_SIZE || headPosition1.y >= height - stoi(settings[8].second) * CELL_SIZE) {
        collision = "bot";
        result = true;
    }
    if (botIsOn) {
        // Проверка на столкновение с границами
        sf::Vector2f headPosition2 = bot.getHeadPosition();
        if (headPosition2.x < stoi(settings[8].second) * CELL_SIZE || headPosition2.x >= width - stoi(settings[8].second) * CELL_SIZE ||
            headPosition2.y < stoi(settings[8].second) * CELL_SIZE || headPosition2.y >= height - stoi(settings[8].second) * CELL_SIZE) {
            collision = (!result) ? "playa" : "Nobody";
            result = true;
        }

        // Проверка на столкновение голов
        float distance = std::hypot(headPosition1.x - headPosition2.x, headPosition1.y - headPosition2.y);
        float minDistance = 0;  // Установите подходящее значение
        if (distance <= minDistance) {
            collision = "Nobody";
            result = true;
        }
        // Проверка столкновения с стенами
        for (const auto& wall : walls) {
            if (headPosition2 == wall.getPosition()) {
                collision = "playa";
                result = true;
            }
        }
        // Проверка на столкновение голов с телом
        if (bot.checkCollisionWithBody(headPosition1)) {
            collision = "bot kill";
            result = true;
        }
        if (bot.checkCollisionWithMyself()) {
            collision = "playa";
            result = true;
        }
        if (snake.checkCollisionWithBody(headPosition2)) {
            collision = "playa kill";
            result = true;
        }
    }
    // Проверка столкновения с стенами
    for (const auto& wall : walls) {
        if (headPosition1 == wall.getPosition()) {
            collision = "bot";
            result = true;
        }
    }
    
    // Проверка на с самими собой
    if (snake.checkCollisionWithMyself()) {
        collision = "bot";
        result = true;
    }

    return result;
}

void singleplayer::map() {

    std::vector<std::pair<std::string, std::string>> settings;

    try {
        settings = getSettings();
    }
    catch (...) {
        setDefaultSettings();
        settings = getSettings();
    }

    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 5; i++)
            addWall(sf::Vector2f(static_cast<int>(rand() % (WIDTH - 2 * stoi(settings[8].second)) + stoi(settings[8].second)) * CELL_SIZE,
                static_cast<int>(rand() % (HEIGHT - 2 * stoi(settings[8].second)) + stoi(settings[8].second)) * CELL_SIZE));
    }
}

void singleplayer::move() {

    // Получаем прошедшее время
    float elapsedTime = clock.getElapsedTime().asSeconds();

    // Копируем голову и добавляем новую голову в новую позицию
    sf::RectangleShape newHead = snake.getBody().front();
    newHead.move(snake.getDirection());

    sf::Vector2f headPosition1 = newHead.getPosition();

    // Проверка на поедание яблока
    if (headPosition1 == apple.getPosition()) {
        snake.eatApple();
        apple.respawn({});
        appleRespawnTimer.restart();
        createSprites();
    }
    else {
        snake.move();
    }

    if(botIsOn)
    {
        sf::RectangleShape newHead2 = bot.getBody().front();
        newHead2.move(bot.getDirection());

        sf::Vector2f headPosition2 = newHead2.getPosition();

        if (headPosition2 == apple.getPosition()) {
            bot.eatApple();
            apple.respawn({});
            appleRespawnTimer.restart();
            createSprites();
        }
        else {
            bot.move();
        }
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

void singleplayer::draw(sf::RenderWindow& window, int& score) {
    // Отрисовка стен и яблока
    drawSprites(window);

    std::vector<std::pair<std::string, std::string>> settings;

    try {
        settings = getSettings();
    }
    catch (...) {
        setDefaultSettings();
        settings = getSettings();
    }

    // Отображение количества клеток змейки
    lengthText.setString("Score: " + std::to_string(score + this->getSnake().getBody().size() - 3));
    lengthText.setPosition(10 + stoi(settings[8].second) * CELL_SIZE, 10 + stoi(settings[8].second) * CELL_SIZE);  // Позиция текста
    window.draw(lengthText);

    // Отрисовка заднего фона (границ)
    window.draw(border);
}


void singleplayer::drawSprites(sf::RenderWindow& window) {

    drawField(window);

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

    std::vector<std::pair<std::string, std::string>> settings;

    try {
        settings = getSettings();
    }
    catch (...) {
        setDefaultSettings();
        settings = getSettings();
    }

    if (!fieldTexture.loadFromFile("../designe/field.png")) {
        std::cerr << "Failed to load field texture." << std::endl;
    }

    if (!snakeTexture.loadFromFile("../designe/snake" + settings[9].second + ".png")) {
        std::cerr << "Failed to load snake texture." << std::endl;
    }

    if (!appleTexture.loadFromFile("../designe/apple" + settings[11].second + ".png")) {
        std::cerr << "Failed to load apple texture." << std::endl;
    }

    if (!wallTexture.loadFromFile("../designe/wall.png")) {
        std::cerr << "Failed to load wall texture." << std::endl;
    }

    if (!botTexture.loadFromFile("../designe/bot" + settings[12].second + ".png")) {
        std::cerr << "Failed to load bots texture." << std::endl;
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
        bodySprites.push_back(segmentSprite);
    }
    if(botIsOn)
    {
        for (const auto& segment : bot.getBody()) {
            sf::Sprite segmentSprite(botTexture);

            segmentSprite.setScale(CELL_SIZE / botTexture.getSize().x, CELL_SIZE / botTexture.getSize().y);
            segmentSprite.setPosition(segment.getPosition());
            bodySprites.push_back(segmentSprite);
        }
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
    std::vector<std::pair<std::string, std::string>> settings;

    try {
        settings = getSettings();
    }
    catch (...) {
        setDefaultSettings();
        settings = getSettings();
    }

    snake.handleInput(window, settings, key);
}

Snake singleplayer::getSnake()
{
    return this->snake;
}

void singleplayer::deleteWalls()
{
    walls.clear();
}

void singleplayer::drawField(sf::RenderWindow& window)
{
    int width = sf::VideoMode::getDesktopMode().width;
    int height = sf::VideoMode::getDesktopMode().height;

    for (int i = 0; (i * CELL_SIZE) < width; ++i) {
        for (int j = 0; (j * CELL_SIZE) < height; ++j) {
            sf::Sprite cellSprite;

            cellSprite.setTexture(fieldTexture);

            cellSprite.setScale(1, 1);
            cellSprite.setPosition(sf::Vector2f(i * CELL_SIZE, j * CELL_SIZE));

            window.draw(cellSprite);
        }
    }
}