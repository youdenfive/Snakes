#include "snake.h"

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

    // ������� ����
    float width = sf::VideoMode::getDesktopMode().width;
    float height = sf::VideoMode::getDesktopMode().height;

    // ������������� �������������� ������
    border.setSize(sf::Vector2f(width - 2 * CELL_SIZE, height - 2 * CELL_SIZE));
    border.setFillColor(sf::Color::Transparent);  // ���������� ����
    border.setOutlineColor(sf::Color::Yellow);      // ���� ������
    border.setOutlineThickness(CELL_SIZE);              // ������� ������

    // ��������� ������� �������������� ������, ����� �� ��������� ������ ������
    border.setPosition(CELL_SIZE, CELL_SIZE);

    // ��������� �����
    if (!font.loadFromFile("../designe/font/menuFont.ttf")) {
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
    if (!font.loadFromFile("../designe/font/menuFont.ttf")) {
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

    // ������� ����
    float width = sf::VideoMode::getDesktopMode().width;
    float height = sf::VideoMode::getDesktopMode().height;

    // �������� �� ������������ � ���������
    sf::Vector2f headPosition = getHeadPosition();
    if (headPosition.x < CELL_SIZE || headPosition.x >= width - CELL_SIZE ||
        headPosition.y < CELL_SIZE || headPosition.y >= height - CELL_SIZE) {
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
            addWall(sf::Vector2f(CELL_SIZE * (2 + i + j * 6), CELL_SIZE * (2 + i + j * 2)));
            addWall(sf::Vector2f(CELL_SIZE * (61 - i - j * 6), CELL_SIZE * (2 + i + j * 2)));
            addWall(sf::Vector2f(CELL_SIZE * (61 - i - j * 6), CELL_SIZE * (33 - i - j * 2)));
            addWall(sf::Vector2f(CELL_SIZE * (2 + i + j * 6), CELL_SIZE * (33 - i - j * 2)));
        }
    }
}