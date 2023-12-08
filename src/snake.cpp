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

    updateSpeed(); // Вызываем для начальной установки интервала обновления

}

void Snake::initFont() {
    // Загружаем шрифт
    if (!font.loadFromFile("../designe/font/menuFont.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
    }
}
void Snake::move() {
    // Получаем прошедшее время
    float elapsedTime = clock.getElapsedTime().asSeconds();

    // Копируем голову и добавляем новую голову в новую позицию
    sf::RectangleShape newHead = body.front();
    newHead.move(direction);

    sf::Vector2f headPosition = newHead.getPosition();

    // Проверка на поедание яблока
    if (headPosition == apple.getPosition()) {
        body.push_front(newHead);
        apple.respawn({});
    }
    else {
        // Удаляем последний сегмент змейки
        body.push_front(newHead);
        body.pop_back();
    }

    // Проверяем, прошло ли достаточно времени для обновления
    if (elapsedTime > updateInterval) {
        clock.restart();

        // Обновляем скорость змейки в зависимости от длины
        updateSpeed();
    }
}


void Snake::grow() {
    // Копируем голову и добавляем ее в начало тела
    body.push_front(body.front());
}

bool Snake::checkWallCollision() {
    sf::Vector2f headPosition = getHeadPosition();

    // Проверка столкновения с границами
    if (headPosition.x < CELL_SIZE || headPosition.x >= (WIDTH - 1) * CELL_SIZE ||
        headPosition.y < CELL_SIZE || headPosition.y >= (HEIGHT - 1) * CELL_SIZE) {
        return true;
    }

    // Проверка столкновения с стенами
    for (const auto& wall : walls) {
        if (headPosition == wall.getPosition()) {
            return true;
        }
    }

    return false;
}



bool Snake::checkCollision() {

    // Размеры окна
    float width = sf::VideoMode::getDesktopMode().width;
    float height = sf::VideoMode::getDesktopMode().height;

    // Проверка на столкновение с границами
    sf::Vector2f headPosition = getHeadPosition();
    if (headPosition.x < CELL_SIZE || headPosition.x >= width - CELL_SIZE ||
        headPosition.y < CELL_SIZE || headPosition.y >= height - CELL_SIZE) {
        return true;
    }

    // Проверка на столкновение с самой собой
    for (auto it = std::next(body.begin()); it != body.end(); ++it) {
        if (it->getPosition() == headPosition) {
            return true;
        }
    }
    // Проверка столкновения с стенами
    for (const auto& wall : walls) {
        if (headPosition == wall.getPosition()) {
            return true;
        }
    }

    return false;
}


void Snake::draw(sf::RenderWindow& window) {

    // Отрисовка стен
    drawWalls(window);

    // Отрисовка каждого сегмента змейки
    for (const auto& segment : body) {
        window.draw(segment);
    }

    // Отрисовка яблока
    sf::RectangleShape appleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    appleShape.setPosition(apple.getPosition());
    appleShape.setFillColor(sf::Color::Red);
    window.draw(appleShape);

    // Отображение количества клеток змейки
    lengthText.setString("Length: " + std::to_string(body.size()));
    lengthText.setPosition(10 + CELL_SIZE, 10 + CELL_SIZE);  // Позиция текста

    window.draw(lengthText);

    // Отрисовка заднего фона (границ)
    window.draw(border);
}


void Snake::handleInput(sf::RenderWindow& window) {
    // Обработка нажатий клавиш
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
    // Пример: более короткий интервал для более длинной змейки098xzxz

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