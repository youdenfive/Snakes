#include "snake.h"

std::size_t Snake::getLength() const {
    return body.size();
}

Snake::Snake() {
    setDefaultSnake();
}

Snake::Snake(sf::Vector2f startPosition, const std::vector<std::pair<std::string, std::string>>& control)
    : _control(control) {
    // ������� ��������� ����� ������
    for (int i = 0; i < 3; ++i) {
        sf::RectangleShape segment(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        segment.setPosition(startPosition);
        segment.setFillColor(sf::Color::Green);
        body.push_front(segment);
    }

    // ��������� ����������� ��������
    direction = sf::Vector2f(0, SPEED);
}

void Snake::setDefaultSnake(sf::Vector2f pos)
{

    body.clear();

    // ������� ��������� ����� ������
    for (int i = 0; i < 3; ++i) {
        sf::RectangleShape segment(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        segment.setPosition(pos);
        segment.setFillColor(sf::Color::Green);
        body.push_front(segment);
    }

    // ��������� ����������� ��������
    direction = sf::Vector2f(0, SPEED);
}

Snake::Snake(const Snake& other): body(other.body) {
    direction = other.direction;
}


void Snake::grow() {
    // �������� ������ � ��������� �� � ������ ����
    body.push_front(body.front());
}




void Snake::draw(sf::RenderWindow& window) {

    // ��������� ������� �������� ������
    for (const auto& segment : body) {
        window.draw(segment);
    }

}


void Snake::handleInput(sf::RenderWindow& window, std::vector<std::pair<std::string, std::string>> control, sf::Keyboard::Scancode key) {
    // ��������� ������� ������
    if (sf::Keyboard::getDescription(key) == control[1].second && direction.x == 0) {
        direction = sf::Vector2f(-SPEED, 0);
    }
    else if (sf::Keyboard::getDescription(key) == control[3].second && direction.x == 0) {
        direction = sf::Vector2f(SPEED, 0);
    }
    else if (sf::Keyboard::getDescription(key) == control[0].second && direction.y == 0) {
        direction = sf::Vector2f(0, -SPEED);
    }
    else if (sf::Keyboard::getDescription(key) == control[2].second && direction.y == 0) {
        direction = sf::Vector2f(0, SPEED);
    }
}

void Snake::handleInputTwo(sf::RenderWindow& window, sf::Keyboard::Scancode key) {
    // ��������� ������� ������
    if (sf::Keyboard::getDescription(key) == _control[1].second && direction.x == 0) {
        direction = sf::Vector2f(-SPEED, 0);
    }
    else if (sf::Keyboard::getDescription(key) == _control[3].second && direction.x == 0) {
        direction = sf::Vector2f(SPEED, 0);
    }
    else if (sf::Keyboard::getDescription(key) == _control[0].second && direction.y == 0) {
        direction = sf::Vector2f(0, -SPEED);
    }
    else if (sf::Keyboard::getDescription(key) == _control[2].second && direction.y == 0) {
        direction = sf::Vector2f(0, SPEED);
    }
}

sf::Vector2f Snake::getHeadPosition() const {
    return body.front().getPosition();
}

void Snake::eatApple() {
    sf::RectangleShape newHead = body.front();
    newHead.move(direction);
    body.push_front(newHead);
};

void Snake::move() {
    sf::RectangleShape newHead = body.front();
    newHead.move(direction);

    // ������� ��������� ������� ������
    body.push_front(newHead);
    body.pop_back();
};
std::deque<sf::RectangleShape> Snake::getBody() {
    return body;
}
sf::Vector2f Snake::getDirection() {
    return direction;
}

bool Snake::checkCollisionWithMyself() {

    sf::Vector2f headPosition = getHeadPosition();

    // ���������, ���� �� ���������� ������� � ���� ������ (����� ������)
    for (std::size_t i = 1; i < body.size(); ++i) {
        if ((body[i].getPosition().x == headPosition.x) && (body[i].getPosition().y == headPosition.y)) {
            return true;
        }
    }

    return false;
}
bool Snake::checkCollisionWithBody(const sf::Vector2f& position) const {
    // ���������, ���� �� ���������� ������� � ���� ������ (����� ������)
    for (std::size_t i = 1; i < body.size(); ++i) {
        if (body[i].getPosition() == position) {
            return true;
        }
    }
    return false;
}
//void Snake::changeBotDirection(const sf::Vector2f& applePosition) {
//    sf::Vector2f targetDirection = applePosition - getHeadPosition();
//    sf::Vector2f newDirection;
//
//    // ���������, ��������� �� ������ ������ ������
//    if (checkCollisionWithBody(applePosition)) {
//        // ���� ������ ��������� ������ ������, ������ ��������� ������� �����������
//        return;
//    }
//
//    // �������� ����������� �� ������ ��������� ������
//    if (std::abs(targetDirection.x) > std::abs(targetDirection.y)) {
//        newDirection = sf::Vector2f((targetDirection.x > 0) ? SPEED : -SPEED, 0);
//    }
//    else {
//        newDirection = sf::Vector2f(0, (targetDirection.y > 0) ? SPEED : -SPEED);
//    }
//
//    // ���������, �� �������� �� ����� ����������� � ������������ � ����� ������
//    if (!checkCollisionWithBody(getHeadPosition() + newDirection)) {
//        direction = newDirection;
//    }
//    else {
//        // ���� ����� ����������� �������� � ������������, ��������� ��������� � ����������� ������
//        if (std::abs(targetDirection.x) > std::abs(targetDirection.y)) {
//            newDirection = sf::Vector2f(0, (targetDirection.y > 0) ? SPEED : -SPEED);
//        }
//        else {
//            newDirection = sf::Vector2f((targetDirection.x > 0) ? SPEED : -SPEED, 0);
//        }
//
//        // ���������, �� �������� �� ����� ����������� � ������������ � ����� ������
//        if (!checkCollisionWithBody(getHeadPosition() + newDirection)) {
//            direction = newDirection;
//        }
//        // ���� � ��� ����������� �������� � ������������, ��������� ������� �����������
//    }
//}
void Snake::changeBotDirection(const sf::Vector2f& applePosition, const std::vector<Wall>& walls) {
    sf::Vector2f targetDirection = applePosition - getHeadPosition();
    sf::Vector2f newDirection;

    // ���������, ��������� �� ������ ������ ������
    if (checkCollisionWithBody(applePosition)) {
        // ���� ������ ��������� ������ ������, ������ ��������� ������� �����������
        return;
    }

    // �������� ����������� �� ������ ��������� ������
    if (std::abs(targetDirection.x) > std::abs(targetDirection.y)) {
        newDirection = sf::Vector2f((targetDirection.x > 0) ? SPEED : -SPEED, 0);
    }
    else {
        newDirection = sf::Vector2f(0, (targetDirection.y > 0) ? SPEED : -SPEED);
    }

    // ���������, �� �������� �� ����� ����������� � ������������ � ����� ������
    // � ������� �� �����
    if (!checkCollisionWithBody(getHeadPosition() + newDirection) && !checkCollisionWithWalls(getHeadPosition() + newDirection, walls)) {
        direction = newDirection;
    }
    else {
        // ���� ����� ����������� �������� � ������������, ��������� ��������� � ����������� ������
        if (std::abs(targetDirection.x) > std::abs(targetDirection.y)) {
            newDirection = sf::Vector2f(0, (targetDirection.y > 0) ? SPEED : -SPEED);
        }
        else {
            newDirection = sf::Vector2f((targetDirection.x > 0) ? SPEED : -SPEED, 0);
        }

        // ���������, �� �������� �� ����� ����������� � ������������ � ����� ������
        // � ������� �� �����
        if (!checkCollisionWithBody(getHeadPosition() + newDirection) && !checkCollisionWithWalls(getHeadPosition() + newDirection, walls)) {
            direction = newDirection;
        }
        // ���� � ��� ����������� �������� � ������������, ��������� ������� �����������
    }
}

bool Snake::checkCollisionWithWalls(const sf::Vector2f& position, const std::vector<Wall>& walls) const {
    for (const auto& wall : walls) {
        if (wall.getPosition().x == position.x && wall.getPosition().y == position.y)
            return true;
    }
    return false;  // ��� ������������ �� ������
}




