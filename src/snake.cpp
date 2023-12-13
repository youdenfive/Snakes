#include "snake.h"

std::size_t Snake::getLength() const {
    return body.size();
}

Snake::Snake() {
    // Создаем начальную длину змейки
    for (int i = 0; i < 3; ++i) {
        sf::RectangleShape segment(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        segment.setPosition(WIDTH / 2 * CELL_SIZE, HEIGHT / 2 * CELL_SIZE + i * CELL_SIZE);
        segment.setFillColor(sf::Color::Green);
        body.push_front(segment);
    }

    // Начальное направление движения
    direction = sf::Vector2f(0, SPEED);
}

Snake::Snake(const Snake& other): body(other.body) {
    direction = other.direction;
}


void Snake::grow() {
    // Копируем голову и добавляем ее в начало тела
    body.push_front(body.front());
}




void Snake::draw(sf::RenderWindow& window) {

    // Отрисовка каждого сегмента змейки
    for (const auto& segment : body) {
        window.draw(segment);
    }

}


void Snake::handleInput(sf::RenderWindow& window, std::vector<std::pair<std::string, std::string>> control, sf::Keyboard::Scancode key) {
    // Обработка нажатий клавиш
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

    // Удаляем последний сегмент змейки
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

    // Проверка на столкновение с самой собой
    for (auto it = std::next(body.begin()); it != body.end(); ++it) {
        if (it->getPosition() == headPosition) {
            return true;
        }
    }

    return false;
}
