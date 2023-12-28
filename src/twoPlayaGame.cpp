#include <SFML/Graphics.hpp>
#include "twoPlayaGame.h"
#include <iostream>
#include <stdlib.h> // ����� ��� ������ ������� rand(), srand()
#include <time.h> // ����� ��� ������ ������� time()

twoPlayaGame::twoPlayaGame(std::string _playa1name, std::string _playa2name) : playa1name(_playa1name), playa2name(_playa2name)
{     
    std::vector<std::pair<std::string, std::string>> settings;

    try {
        settings = getSettings();
    }
    catch (...) {
        setDefaultSettings();
        settings = getSettings();
    }

   player1 = Snake(sf::Vector2f(WIDTH / 3 * CELL_SIZE, HEIGHT / 2 * CELL_SIZE), { settings[0], settings[1], settings[2], settings[3] });
   player2 = Snake(sf::Vector2f(2 * WIDTH / 3 * CELL_SIZE, HEIGHT / 2 * CELL_SIZE), { settings[13], settings[14], settings[15], settings[16] });

    collision = "";
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
    score = 0;
};

int twoPlayaGame::startTwoPlayaGame(sf::RenderWindow& window)
{

    window.setFramerateLimit(10);

    // ������������� ���������� ��������� �����
    srand(static_cast<unsigned>(time(nullptr)));

    initTextures();
    createSprites();

    // ��������� ���������� ��������� ����������
    updateSpeed();

    // �������������� ����� ��������� ����
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
        setDefaultSettings();
        settings = getSettings();
    }

    auto roundsCount = std::stoi(settings[4].second);

    int score1 = 0;
    int score2 = 0;

    // �������������� ����� ��������� ����
    std::vector<sf::String> gameOverName = { "PLAY AGAIN", "BACK TO MENU" };
    gameMenu gameOverMenu(window, 950, 550, gameOverName, 100, 70);
    gameOverMenu.setColor(sf::Color::Red, sf::Color(220, 220, 220), sf::Color::Red);
    gameOverMenu.alignTextMenu(1);


    sf::Event event;

    for (int roundNumber = 1; roundNumber <= roundsCount; ++roundNumber) {
        collision = "";

        sf::Text round;
        round.setFont(font);
        round.setCharacterSize(30);
        round.setFillColor(sf::Color::White);
        round.setPosition(1560, 10 + stoi(settings[8].second) * CELL_SIZE);
        round.setString("Round: " + std::to_string(roundNumber) + '/' + std::to_string(roundsCount));

        player1.setDefaultSnake(sf::Vector2f(WIDTH / 3 * CELL_SIZE, HEIGHT / 2 * CELL_SIZE));
        player2.setDefaultSnake(sf::Vector2f(2 * WIDTH / 3 * CELL_SIZE, HEIGHT / 2 * CELL_SIZE));

        while (window.isOpen()) {
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

            // ���������� ��������� ������, ����� �������� ������������ � �������
            getApple().respawn(getWalls());

            move();

            updateSpeed(); // �������� ��� ��������� ��������� ��������� ����������



            if (checkCollision()) {
                score1 = score1 + getPlayer1().getBody().size() - 3;
                score2 = score2 + getPlayer2().getBody().size() - 3;

                if (collision == playa1name) {
                    score1 += 5;
                }
                else if (collision == playa2name) {
                    score2 += 5;
                }
                else if (collision == playa1name + " kill") {
                    score1 += 10;
                }
                else if (collision == playa2name + " kill") {
                    score2 += 10;
                }
                break;
            }

            // ���������
            window.clear();
            draw(window, sf::Vector2i(score1, score2));
            window.draw(round);
            window.display();
        }
    }
    while (true) {
        window.clear();


        int winScore = (score1 > score2) ? score1 : score2;
        std::string winner = (score1 == winScore) ? ((score2 == winScore) ? "nobody" : playa1name) : playa2name;
        score = winScore;

        sf::RectangleShape winnerCol;

        //if (score1 != score2) {

            std::string winnerColor = (score1 > score2) ? settings[9].second : settings[10].second;

            sf::Texture winnerTexture;
            std::string winnerTexturePath = "../designe/snake" + winnerColor + ".png";
            if (!winnerTexture.loadFromFile(winnerTexturePath)) return -1;
            winnerCol.setSize(sf::Vector2f(30, 30));
            winnerCol.setPosition(sf::Vector2f(730, 440));
            winnerCol.setTexture(&winnerTexture);
        //}
        if (score1 == score2) {
            winnerCol.setSize(sf::Vector2f(0, 0));
        }

        // ����������� "Game Over" � ����� ������
        sf::Text gameOverText;
        gameOverText.setFont(getFont());
        gameOverText.setCharacterSize(40);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setString(winner + " win!");
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
        window.draw(winnerCol);
        gameOverMenu.draw();

        window.display();
    }


    return score;
}

void twoPlayaGame::initFont() {
    // ��������� �����
    if (!font.loadFromFile("../designe/font/menuFont.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
    }
}

void twoPlayaGame::updateSpeed() {
    // ������: ����� �������� �������� ��� ����� ������� ������

    updateInterval = std::max(0.1f, 1.0f - 0.05f * static_cast<float>(player1.getBody().size()));
}

bool twoPlayaGame::checkCollision() {
    bool result = false;

    std::vector<std::pair<std::string, std::string>> settings;

    try {
        settings = getSettings();
    }
    catch (...) {
        setDefaultSettings();
        settings = getSettings();
    }

    // ������� ����
    float width = sf::VideoMode::getDesktopMode().width;
    float height = sf::VideoMode::getDesktopMode().height;

    // �������� �� ������������ � ���������
    sf::Vector2f headPosition1 = player1.getHeadPosition();
    if (headPosition1.x < stoi(settings[8].second) * CELL_SIZE || headPosition1.x >= width - stoi(settings[8].second) * CELL_SIZE ||
        headPosition1.y < stoi(settings[8].second) * CELL_SIZE || headPosition1.y >= height - stoi(settings[8].second) * CELL_SIZE) {
        collision = playa2name;
        result = true;
    }
    // �������� �� ������������ � ���������
    sf::Vector2f headPosition2 = player2.getHeadPosition();
    if (headPosition2.x < stoi(settings[8].second) * CELL_SIZE || headPosition2.x >= width - stoi(settings[8].second) * CELL_SIZE ||
        headPosition2.y < stoi(settings[8].second) * CELL_SIZE || headPosition2.y >= height - stoi(settings[8].second) * CELL_SIZE) {
        collision = (!result) ? playa1name : "Nobody";
        result = true;
    }

    // �������� �� ������������ �����
    float distance = std::hypot(headPosition1.x - headPosition2.x, headPosition1.y - headPosition2.y);
    float minDistance = 0;  // ���������� ���������� ��������
    if (distance <= minDistance) {
        collision = "Nobody";
        result = true;
    }

    // �������� �� ������������ ����� � �����
    if (player2.checkCollisionWithBody(headPosition1)) {
        collision = playa2name + " kill";
        result = true;
    }

    if (player1.checkCollisionWithBody(headPosition2)) {
        collision = playa1name + " kill";
        result = true;
    }
    // �������� �� � ������ �����
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

    // �������� ��������� �����
    float elapsedTime = clock.getElapsedTime().asSeconds();

    // �������� ������ � ��������� ����� ������ � ����� �������
    sf::RectangleShape newHead = player1.getBody().front();
    newHead.move(player1.getDirection());

    sf::Vector2f headPosition = newHead.getPosition();

    sf::RectangleShape newHead2 = player2.getBody().front();
    newHead2.move(player2.getDirection());

    sf::Vector2f headPosition2 = newHead2.getPosition();

    // �������� �� �������� ������
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

    // �������� ������� � ������� ���������� �������� ������
    if (appleRespawnTimer.getElapsedTime().asSeconds() >= 5) {
        // ������� ������ � ����� �������
        appleSkinChangerFunction2023AkaMultyplySomeSkinsTogether();
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

void twoPlayaGame::draw(sf::RenderWindow& window, sf::Vector2i playersScore) {
    // ��������� ���� � ������
    drawSprites(window);

    std::vector<std::pair<std::string, std::string>> settings;

    try {
        settings = getSettings();
    }
    catch (...) {
        setDefaultSettings();
        settings = getSettings();
    }

    // ����������� ���������� ������ ������
    lengthText.setString(playa1name + ":" + std::to_string(playersScore.x + getPlayer1().getBody().size() - 3));
    lengthText.setPosition(50 + stoi(settings[8].second) * CELL_SIZE, 10 + stoi(settings[8].second) * CELL_SIZE);  // ������� ������
    window.draw(lengthText);

    // ����������� ���������� ������ ������
    lengthText.setString(playa2name + ":" + std::to_string(playersScore.y + getPlayer2().getBody().size() - 3));
    lengthText.setPosition(50 + stoi(settings[8].second) * CELL_SIZE, 10 + (stoi(settings[8].second) + 1) * CELL_SIZE);  // ������� ������
    window.draw(lengthText);


    sf::RectangleShape player1;
    sf::Texture player1Texture;
    std::string player1TexturePath = "../designe/snake" + settings[9].second + ".png";
    if (!player1Texture.loadFromFile(player1TexturePath)) return;
    player1.setSize(sf::Vector2f(30, 30));
    player1.setPosition(sf::Vector2f(10 + stoi(settings[8].second) * CELL_SIZE, 10 + stoi(settings[8].second) * CELL_SIZE));
    player1.setTexture(&player1Texture);
    window.draw(player1);

    sf::RectangleShape player2;
    sf::Texture player2Texture;
    std::string player2TexturePath = "../designe/snake" + settings[10].second + ".png";
    if (!player2Texture.loadFromFile(player2TexturePath)) return;
    player2.setSize(sf::Vector2f(30, 30));
    player2.setPosition(sf::Vector2f(10 + stoi(settings[8].second) * CELL_SIZE, 10 + (stoi(settings[8].second) + 1) * CELL_SIZE));
    player2.setTexture(&player2Texture);
    window.draw(player2);

    // ��������� ������� ���� (������)
    window.draw(border);
}


void twoPlayaGame::drawSprites(sf::RenderWindow& window) {
    // ��������� ������� �������� ������ � ������ ��������

    drawField(window);

    for (const auto& sprite : bodySprites) {
        window.draw(sprite);
    }

    // ��������� ������
    window.draw(appleSprite);
}

void twoPlayaGame::initTextures() {

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

    if (!snake1Texture.loadFromFile("../designe/snake" + settings[9].second + ".png")) {
        std::cerr << "Failed to load snake texture." << std::endl;
    }

    if (!snake2Texture.loadFromFile("../designe/snake" + settings[10].second + ".png")) {
        std::cerr << "Failed to load snake texture." << std::endl;
    }

    if (!appleTexture.loadFromFile("../designe/apple" + settings[11].second + ".png")) {
        std::cerr << "Failed to load apple texture." << std::endl;
    }
    
    appleSkinChangerFunction2023AkaMultyplySomeSkinsTogether();
}

void twoPlayaGame::createSprites() {

    appleSprite.setTexture(appleTexture);
    appleSprite.setScale(CELL_SIZE / appleTexture.getSize().x, CELL_SIZE / appleTexture.getSize().y);
    appleSprite.setPosition(apple.getPosition());
    for (const auto& segment : player1.getBody()) {
        sf::Sprite segmentSprite(snake1Texture);
        segmentSprite.setScale(CELL_SIZE / snake1Texture.getSize().x, CELL_SIZE / snake1Texture.getSize().y);
        segmentSprite.setPosition(segment.getPosition());
        bodySprites.push_back(segmentSprite);
    }

    for (const auto& segment : player2.getBody()) {
        sf::Sprite segmentSprite(snake2Texture);
        segmentSprite.setScale(CELL_SIZE / snake2Texture.getSize().x, CELL_SIZE / snake2Texture.getSize().y);
        segmentSprite.setPosition(segment.getPosition());
        bodySprites.push_back(segmentSprite);
    }
}

void twoPlayaGame::handleInput(sf::RenderWindow& window, sf::Keyboard::Scancode key, Snake& snake)
{
    snake.handleInputTwo(window, key);

}

void twoPlayaGame::drawField(sf::RenderWindow& window)
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