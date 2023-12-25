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

    int score = 0;

    sf::Text nickname;
    nickname.setFont(font);
    nickname.setCharacterSize(30);
    nickname.setFillColor(sf::Color::White);
    nickname.setPosition(860, 10 + CELL_SIZE);
    nickname.setString(settings[6].second);

    // ������������� ���������� ��������� �����
    srand(static_cast<unsigned>(time(nullptr)));
    int roundsCount = stoi(settings[4].second);

    for (int roundNumber = 1; roundNumber <= roundsCount; ++roundNumber) {
        Snake snake;
        singleplayer game(snake);

        sf::Text round;
        round.setFont(font);
        round.setCharacterSize(30);
        round.setFillColor(sf::Color::White);
        round.setPosition(1560, 10 + CELL_SIZE);
        round.setString("Round: " + std::to_string(roundNumber) + '/' + std::to_string(roundsCount));

        // ���������� ����
        game.map();
        game.initTextures();
        game.createSprites();
        // ��������� ���������� ��������� ����������
        game.updateSpeed();

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

            if (sf::Event::KeyReleased) {

                auto key = event.key.scancode;

                // ����� ��������� ������� �����, ���������� ��������� ������
                game.handleInput(window, key);
            }

            // ���������� ��������� ������, ����� �������� ������������ � �������
            game.getApple().respawn(game.getWalls());

            // �������� ������
            game.move();

            updateSpeed(); // �������� ��� ��������� ��������� ��������� ����������

            // �������������� ����� ��������� ����
            std::vector<sf::String> gameOverName = { "PLAY AGAIN", "BACK TO MENU" };
            gameMenu gameOverMenu(window, 950, 550, gameOverName, 100, 70);
            gameOverMenu.setColor(sf::Color::Red, sf::Color(220, 220, 220), sf::Color::Red);
            gameOverMenu.alignTextMenu(1);

            if (game.checkCollision() && roundNumber < roundsCount) {
                score = score + game.getSnake().getBody().size() - 3;
                break;
            }


            // �������� �� ������������
            while (game.checkCollision()) {

                std::cout << "Game Over!" << std::endl;

                window.clear();

                // ����������� "Game Over" � ����� ������
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
                lengthText.setString("Score: " + std::to_string(score + game.getSnake().getBody().size() - 3));
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
                                //game.startSingleplayer(window);
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
            window.draw(nickname);
            window.draw(round);
            game.draw(window, score);
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

    // �������� �� ������������ � ���������
    sf::Vector2f headPosition = snake.getHeadPosition();
    if (headPosition.x < CELL_SIZE || headPosition.x >= width - CELL_SIZE ||
        headPosition.y < CELL_SIZE || headPosition.y >= height - CELL_SIZE) {
        return true;
    }

    // �������� ������������ � �������
    for (const auto& wall : walls) {
        if (headPosition == wall.getPosition()) {
            return true;
        }
    }

    // �������� �� ������������ � ����� �����
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

    // �������� ��������� �����
    float elapsedTime = clock.getElapsedTime().asSeconds();

    // �������� ������ � ��������� ����� ������ � ����� �������
    sf::RectangleShape newHead = snake.getBody().front();
    newHead.move(snake.getDirection());

    sf::Vector2f headPosition = newHead.getPosition();

    // �������� �� �������� ������
    if (headPosition == apple.getPosition()) {
        snake.eatApple();
        apple.respawn({});
        appleRespawnTimer.restart();
        createSprites();
    }
    else {
        // ������� ��������� ������� ������
        snake.move();
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

    // ����������� ���������� ������ ������
    lengthText.setString("Score: " + std::to_string(score + this->getSnake().getBody().size() - 3));
    lengthText.setPosition(10 + CELL_SIZE, 10 + CELL_SIZE);  // ������� ������
    window.draw(lengthText);

    // ��������� ������� ���� (������)
    window.draw(border);
}


void singleplayer::drawSprites(sf::RenderWindow& window) {
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
    if (!snakeTexture.loadFromFile("../designe/snake2.png")) {
        std::cerr << "Failed to load snake texture." << std::endl;
    }

    if (!appleTexture.loadFromFile("../designe/apple1.png")) {
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
        //segmentSprite.setRotation(rotation);  // ������������� ���� ��������
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