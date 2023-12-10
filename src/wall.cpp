#include "Wall.h"

Wall::Wall(sf::Vector2f position) : position(position) {
    sf::Texture wallTexture;
    if (!wallTexture.loadFromFile("wall.png")) {
        std::cout << "Failed to load wall texture." << std::endl;
    }

    sprite.setTexture(wallTexture);
    sprite.setPosition(position);
}

const sf::Vector2f& Wall::getPosition() const {
    return position;
}

const sf::Sprite& Wall::getSprite() const {
    return sprite;
}