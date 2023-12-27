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
        settings = setDefaultSettings();
    }

    // ������� ����
    float width = sf::VideoMode::getDesktopMode().width;
    float height = sf::VideoMode::getDesktopMode().height;

    // ������������� �������������� ������
    border.setSize(sf::Vector2f(width - 2 * stoi(settings[8].second) * CELL_SIZE, height - 2 * stoi(settings[8].second) * CELL_SIZE));
    border.setFillColor(sf::Color::Transparent);  // ���������� ����
    border.setOutlineColor(sf::Color::Yellow);      // ���� ������
    border.setOutlineThickness(stoi(settings[8].second) * CELL_SIZE);              // ������� ������

    // ��������� ������� �������������� ������, ����� �� ��������� ������ ������
    border.setPosition(stoi(settings[8].second) * CELL_SIZE, stoi(settings[8].second) * CELL_SIZE);

    // ��������� �����
    if (!font.loadFromFile("../designe/font/menuFont.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
    }

    // ��������� ���������� ������
    lengthText.setFont(font);
    lengthText.setCharacterSize(20);
    lengthText.setFillColor(sf::Color::White);

    initFont();  // �������� ����� ��� ������������� ������
};

int singleplayer::startSingleplayer(sf::RenderWindow& window)
{

    window.setFramerateLimit(10);

    std::vector<std::pair<std::string, std::string>> settings;

    try {
        settings = getSettings();
    }
    catch (...) {
        settings = setDefaultSettings();
    }

    if (settings[5].second == "ON") {
        botIsOn = true;
    }
    else {
        botIsOn = false;
    }

    int score = 0;

    sf::Text nickname;
    nickname.setFont(font);
    nickname.setCharacterSize(30);
    nickname.setFillColor(sf::Color::White);
    nickname.setPosition(860, 10 + stoi(settings[8].second) * CELL_SIZE);
    nickname.setString(settings[6].second);

    // ������������� ���������� ��������� �����
    srand(static_cast<unsigned>(time(nullptr)));
    int roundsCount = stoi(settings[4].second);

    for (int roundNumber = 1; roundNumber <= roundsCount; ++roundNumber) {

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
        // ���������� ����
        map();
        initTextures();
        createSprites();
        // ��������� ���������� ��������� ����������
        updateSpeed();

        // �������������� ����� ��������� ����
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
                            // ����� ����������� ������.
                            if (event.key.code == sf::Keyboard::Up) {
                                gamePauseMenu.moveUp();
                            }

                            // ����� ����������� ������.
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

                // ����� ��������� ������� �����, ���������� ��������� ������
                handleInput(window, key);
            }

            // ���������� ��������� ������, ����� �������� ������������ � �������
            getApple().respawn(getWalls());

            if (botIsOn) {
                bot.changeBotDirection(apple.getPosition(), walls);
            }

            // �������� ������
            move();

            updateSpeed(); // �������� ��� ��������� ��������� ��������� ����������

            // �������������� ����� ��������� ����
            std::vector<sf::String> gameOverName = { "PLAY AGAIN", "BACK TO MENU" };
            gameMenu gameOverMenu(window, 950, 550, gameOverName, 100, 70);
            gameOverMenu.setColor(sf::Color::Red, sf::Color(220, 220, 220), sf::Color::Red);
            gameOverMenu.alignTextMenu(1);

            if (checkCollision() && roundNumber < roundsCount) {
                score = score + getSnake().getBody().size() - 3;
                break;
            }


            // �������� �� ������������
            while (checkCollision()) {

                std::cout << "Game Over!" << std::endl;

                window.clear();

                // ����������� "Game Over" � ����� ������
                sf::Text gameOverText;
                gameOverText.setFont(getFont());
                gameOverText.setCharacterSize(40);
                gameOverText.setFillColor(sf::Color::Red);
                gameOverText.setString("Game Over!");
                gameOverText.setPosition(WIDTH * CELL_SIZE / 2 - 100, HEIGHT * CELL_SIZE / 2 - 20);

                sf::Text lengthText;
                lengthText.setFont(getFont());
                lengthText.setCharacterSize(20);
                lengthText.setFillColor(sf::Color::White);
                lengthText.setString("Score: " + std::to_string(score + getSnake().getBody().size() - 3));
                lengthText.setPosition(WIDTH * CELL_SIZE / 2 - 20, HEIGHT * CELL_SIZE / 2 + 40);

                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
                        window.close();
                        return EXIT_SUCCESS;
                    }

                    // ������������ ������� ������.
                    if (event.type == sf::Event::KeyReleased) {

                        // ����� ����������� ������.
                        if (event.key.code == sf::Keyboard::Up) {
                            gameOverMenu.moveUp();
                        }

                        // ����� ����������� ������.
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

            // ���������
            window.clear();
            draw(window, score);
            window.draw(nickname);
            window.draw(round);

            window.display();
        }
    }
    return snake.getLength();
}

void singleplayer::initFont() {
    // ��������� �����
    if (!font.loadFromFile("../designe/font/menuFont.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
    }
}

void singleplayer::updateSpeed() {
    // ������: ����� �������� �������� ��� ����� ������� ������

    updateInterval = std::max(0.1f, 1.0f - 0.05f * static_cast<float>(snake.getBody().size()));
}

bool singleplayer::checkCollision() {

    // ������� ����
    float width = sf::VideoMode::getDesktopMode().width;
    float height = sf::VideoMode::getDesktopMode().height;

    std::vector<std::pair<std::string, std::string>> settings;

    try {
        settings = getSettings();
    }
    catch (...) {
        settings = setDefaultSettings();
    }

    // �������� �� ������������ � ���������
    sf::Vector2f headPosition1 = snake.getHeadPosition();
    if (headPosition1.x < stoi(settings[8].second) * CELL_SIZE || headPosition1.x >= width - stoi(settings[8].second) * CELL_SIZE ||
        headPosition1.y < stoi(settings[8].second) * CELL_SIZE || headPosition1.y >= height - stoi(settings[8].second) * CELL_SIZE) {
        return true;
    }
    if (botIsOn) {
        // �������� �� ������������ � ���������
        sf::Vector2f headPosition2 = bot.getHeadPosition();
        if (headPosition2.x < stoi(settings[8].second) * CELL_SIZE || headPosition2.x >= width - stoi(settings[8].second) * CELL_SIZE ||
            headPosition2.y < stoi(settings[8].second) * CELL_SIZE || headPosition2.y >= height - stoi(settings[8].second) * CELL_SIZE) {
            return true;
        }

        // �������� �� ������������ �����
        float distance = std::hypot(headPosition1.x - headPosition2.x, headPosition1.y - headPosition2.y);
        float minDistance = 0;  // ���������� ���������� ��������
        if (distance <= minDistance) {
            return true;
        }
        // �������� ������������ � �������
        for (const auto& wall : walls) {
            if (headPosition2 == wall.getPosition()) {
                return true;
            }
        }
        // �������� �� ������������ ����� � �����
        if (bot.checkCollisionWithBody(headPosition1)) {
            return true;
        }
        if (bot.checkCollisionWithMyself()) {
            return true;
        }
        if (snake.checkCollisionWithBody(headPosition2)) {
            return true;
        }
    }
    // �������� ������������ � �������
    for (const auto& wall : walls) {
        if (headPosition1 == wall.getPosition()) {
            return true;
        }
    }
    
    // �������� �� � ������ �����
    if (snake.checkCollisionWithMyself()) {
        return true;
    }

    return false;
}

void singleplayer::map() {

    std::vector<std::pair<std::string, std::string>> settings;

    try {
        settings = getSettings();
    }
    catch (...) {
        settings = setDefaultSettings();
    }

    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 5; i++)
            addWall(sf::Vector2f(static_cast<int>(rand() % (WIDTH - 2 * stoi(settings[8].second)) + stoi(settings[8].second)) * CELL_SIZE,
                static_cast<int>(rand() % (HEIGHT - 2 * stoi(settings[8].second)) + stoi(settings[8].second)) * CELL_SIZE));
    }
}

void singleplayer::move() {

    // �������� ��������� �����
    float elapsedTime = clock.getElapsedTime().asSeconds();

    // �������� ������ � ��������� ����� ������ � ����� �������
    sf::RectangleShape newHead = snake.getBody().front();
    newHead.move(snake.getDirection());

    sf::Vector2f headPosition1 = newHead.getPosition();

    // �������� �� �������� ������
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

    

    // �������� ������� � ������� ���������� �������� ������
    if (appleRespawnTimer.getElapsedTime().asSeconds() >= 5) {
        // ������� ������ � ����� �������
        apple.respawn(getWalls());
        appleRespawnTimer.restart();
    }

    // ���������, ������ �� ���������� ������� ��� ����������
    if (elapsedTime > updateInterval) {
        clock.restart();

        // ��������� �������� ������ � ����������� �� �����
        updateSpeed();
    }
    // ������� ������ �������� ����� �����������
    bodySprites.clear();
    createSprites();
}

void singleplayer::draw(sf::RenderWindow& window, int& score) {
    // ��������� ���� � ������
    drawSprites(window);

    std::vector<std::pair<std::string, std::string>> settings;

    try {
        settings = getSettings();
    }
    catch (...) {
        settings = setDefaultSettings();
    }

    // ����������� ���������� ������ ������
    lengthText.setString("Score: " + std::to_string(score + this->getSnake().getBody().size() - 3));
    lengthText.setPosition(10 + stoi(settings[8].second) * CELL_SIZE, 10 + stoi(settings[8].second) * CELL_SIZE);  // ������� ������
    window.draw(lengthText);

    // ��������� ������� ���� (������)
    window.draw(border);
}


void singleplayer::drawSprites(sf::RenderWindow& window) {

    drawField(window);

    // ��������� ����
    for (const auto& wall : walls) {
        window.draw(wall.getSprite());
    }

    // ��������� ������� �������� ������ � ������ ��������
    for (const auto& sprite : bodySprites) {
        window.draw(sprite);
    }

    // ��������� ������
    window.draw(appleSprite);
}

void singleplayer::initTextures() {

    std::vector<std::pair<std::string, std::string>> settings;

    try {
        settings = getSettings();
    }
    catch (...) {
        settings = setDefaultSettings();
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
        settings = setDefaultSettings();
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