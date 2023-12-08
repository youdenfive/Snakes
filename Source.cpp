#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <deque>

// ������ ����
const int WIDTH = 60;
const int HEIGHT = 30;

// ������ ������
const float CELL_SIZE = 30.0f;

// �������� ������
const float SPEED = CELL_SIZE;

class Wall {
public:
    Wall(sf::Vector2f position) : position(position) {
        shape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        shape.setPosition(position);
        shape.setFillColor(sf::Color::Blue);  // ���� ����
    }

    const sf::Vector2f& getPosition() const {
        return position;
    }

    void draw(sf::RenderWindow& window) const  {
        window.draw(shape);
    }

private:
    sf::Vector2f position;
    sf::RectangleShape shape;
};


class Apple {
public:
    Apple();
    sf::Vector2f getPosition() const;
    void respawn(const std::vector<Wall>& walls);
    bool isOnWall(const std::vector<Wall>& walls) const;

private:
    sf::Vector2f position;
};

Apple::Apple() {
    respawn({});
}

sf::Vector2f Apple::getPosition() const {
    return position;
}

void Apple::respawn(const std::vector<Wall>& walls) {
    // ���������� ��������� ���������� ��� ������ � �������� ����
    do {
        position.x = static_cast<float>(rand() % (WIDTH - 2) + 1) * CELL_SIZE;
        position.y = static_cast<float>(rand() % (HEIGHT - 2) + 1) * CELL_SIZE;
    } while (isOnWall(walls));
}
bool Apple::isOnWall(const std::vector<Wall>& walls) const {
    // ���������, ��������� �� ������ �� ������
    for (const auto& wall : walls) {
        if (position == wall.getPosition()) {
            return true;
        }
    }
    return false;
}

class Snake {
public:
    Snake();
    void move();
    void grow();
    bool checkCollision();
    void handleInput(sf::RenderWindow& window);
    sf::Vector2f getHeadPosition() const;
    void draw(sf::RenderWindow& window);
    void updateSpeed();
    const sf::Font& getFont() const {
        return font;
    }
    std::size_t getLength() const;
    void addWall(sf::Vector2f position) {
        walls.emplace_back(position);
    }

    void drawWalls(sf::RenderWindow& window) {
        for (const auto& wall : walls) {
            wall.draw(window);
        }
    }
    bool checkWallCollision();
    void map();
    const std::vector<Wall>& getWalls() const {
        return walls;
    }
    Apple getApple() const{
        return apple;
    }

private:
    std::deque<sf::RectangleShape> body;
    sf::Vector2f direction;
    Apple apple;  // ��������� ������ Apple

    sf::Font font;         // ����������� ������ ������
    sf::Text lengthText;   // ����������� ������ ������ ��� ����������� ���������� ������

    sf::Clock clock;        // ����������� ������ ����� ��� ��������� �������
    float updateInterval;   // ����������� ���� ��� �������� ��������� ����������
    sf::RectangleShape border;  // ����������� ������ �������������� ��� ������
    void initFont();
    std::vector<Wall> walls;
};

std::size_t Snake::getLength() const {
    return body.size();
}

Snake::Snake() {
    // ������� ��������� ����� ������
    for (int i = 0; i < 3; ++i) {
        sf::RectangleShape segment(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        segment.setPosition(WIDTH / 2 * CELL_SIZE, HEIGHT / 2 * CELL_SIZE + i * CELL_SIZE);
        segment.setFillColor(sf::Color::Green);
        body.push_front(segment);
    }
    
    // ��������� ����������� ��������
    direction = sf::Vector2f(0, SPEED);

    // ������������� �������������� ������
    border.setSize(sf::Vector2f(WIDTH * CELL_SIZE - 2 * CELL_SIZE, HEIGHT * CELL_SIZE - 2 * CELL_SIZE));
    border.setFillColor(sf::Color::Transparent);  // ���������� ����
    border.setOutlineColor(sf::Color::Yellow);      // ���� ������
    border.setOutlineThickness(CELL_SIZE);              // ������� ������

    // ��������� ������� �������������� ������, ����� �� ��������� ������ ������
    border.setPosition(CELL_SIZE, CELL_SIZE);

    // ��������� �����
    if (!font.loadFromFile("menuFont.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
    }

    // ��������� ���������� ������
    lengthText.setFont(font);
    lengthText.setCharacterSize(20);
    lengthText.setFillColor(sf::Color::White);

    initFont();  // �������� ����� ��� ������������� ������

    updateSpeed(); // �������� ��� ��������� ��������� ��������� ����������

}

void Snake::initFont() {
    // ��������� �����
    if (!font.loadFromFile("menuFont.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
    }
}
void Snake::move() {
    // �������� ��������� �����
    float elapsedTime = clock.getElapsedTime().asSeconds();

    // �������� ������ � ��������� ����� ������ � ����� �������
    sf::RectangleShape newHead = body.front();
    newHead.move(direction);

    sf::Vector2f headPosition = newHead.getPosition();

    // �������� �� �������� ������
    if (headPosition == apple.getPosition()) {
        body.push_front(newHead);
        apple.respawn({});
    }
    else {
        // ������� ��������� ������� ������
        body.push_front(newHead);
        body.pop_back();
    }

    // ���������, ������ �� ���������� ������� ��� ����������
    if (elapsedTime > updateInterval) {
        clock.restart();

        // ��������� �������� ������ � ����������� �� �����
        updateSpeed();
    }
}


void Snake::grow() {
    // �������� ������ � ��������� �� � ������ ����
    body.push_front(body.front());
}

bool Snake::checkWallCollision() {
    sf::Vector2f headPosition = getHeadPosition();

    // �������� ������������ � ���������
    if (headPosition.x < CELL_SIZE || headPosition.x >= (WIDTH - 1) * CELL_SIZE ||
        headPosition.y < CELL_SIZE || headPosition.y >= (HEIGHT - 1) * CELL_SIZE) {
        return true;
    }

    // �������� ������������ � �������
    for (const auto& wall : walls) {
        if (headPosition == wall.getPosition()) {
            return true;
        }
    }

    return false;
}



bool Snake::checkCollision() {
    // �������� �� ������������ � ���������
    sf::Vector2f headPosition = getHeadPosition();
    if (headPosition.x < CELL_SIZE || headPosition.x >= (WIDTH - 1) * CELL_SIZE ||
        headPosition.y < CELL_SIZE || headPosition.y >= (HEIGHT - 1) * CELL_SIZE) {
        return true;
    }

    // �������� �� ������������ � ����� �����
    for (auto it = std::next(body.begin()); it != body.end(); ++it) {
        if (it->getPosition() == headPosition) {
            return true;
        }
    }
    // �������� ������������ � �������
    for (const auto& wall : walls) {
        if (headPosition == wall.getPosition()) {
            return true;
        }
    }

    return false;
}


void Snake::draw(sf::RenderWindow& window) {

    // ��������� ����
    drawWalls(window);

    // ��������� ������� �������� ������
    for (const auto& segment : body) {
        window.draw(segment);
    }

    // ��������� ������
    sf::RectangleShape appleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    appleShape.setPosition(apple.getPosition());
    appleShape.setFillColor(sf::Color::Red);
    window.draw(appleShape);

    // ����������� ���������� ������ ������
    lengthText.setString("Length: " + std::to_string(body.size()));
    lengthText.setPosition(10 + CELL_SIZE, 10 + CELL_SIZE);  // ������� ������

    window.draw(lengthText);

    // ��������� ������� ���� (������)
    window.draw(border);
}


void Snake::handleInput(sf::RenderWindow& window) {
    // ��������� ������� ������
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && direction.x == 0) {
        direction = sf::Vector2f(-SPEED, 0);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && direction.x == 0) {
        direction = sf::Vector2f(SPEED, 0);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && direction.y == 0) {
        direction = sf::Vector2f(0, -SPEED);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && direction.y == 0) {
        direction = sf::Vector2f(0, SPEED);
    }
}

sf::Vector2f Snake::getHeadPosition() const {
    return body.front().getPosition();
}

void Snake::updateSpeed() {
    // ������: ����� �������� �������� ��� ����� ������� ������098xzxz

    updateInterval = std::max(0.1f, 1.0f - 0.05f * static_cast<float>(body.size()));
}
void Snake::map() {
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 5; i++) {
            addWall(sf::Vector2f(CELL_SIZE * (2 + i + j*6), CELL_SIZE * (2 + i + j * 2)));
            addWall(sf::Vector2f(CELL_SIZE * (57 - i - j * 6), CELL_SIZE * (2 + i + j * 2)));
            addWall(sf::Vector2f(CELL_SIZE * (57 - i - j * 6), CELL_SIZE * (27 - i - j * 2)));
            addWall(sf::Vector2f(CELL_SIZE * (2 + i + j * 6), CELL_SIZE * (27 - i - j * 2)));
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE), "Snake Game");
    window.setFramerateLimit(10);

    // ������������� ���������� ��������� �����
    srand(static_cast<unsigned>(time(nullptr)));

    Snake snake;

    // ���������� ����
    snake.map();

    // ��������� ���������� ��������� ����������
    snake.updateSpeed();
    snake.draw(window);  // ������ ������� ����� �������� ������

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }

        // ����� ��������� ������� �����, ���������� ��������� ������
        snake.handleInput(window);

        // ���������� ��������� ������, ����� �������� ������������ � �������
        snake.getApple().respawn(snake.getWalls());

        // �������� ������
        snake.move();

        // �������� �� ������������
        if (snake.checkCollision()) {
            std::cout << "Game Over!" << std::endl;

            sf::Clock clock;
            while (clock.getElapsedTime().asSeconds() < 4) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                        return EXIT_SUCCESS;
                    }
                }
                window.clear();

                // ����������� "Game Over" � ����� ������
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

                window.draw(gameOverText);
                window.draw(lengthText);

                window.display();
            }
            window.close();
        }

        // ���������
        window.clear();
        snake.draw(window);
        window.display();
    }

    return EXIT_SUCCESS;
}
