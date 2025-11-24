#include "AnimatedSprite.h"
#include <iostream>

AnimatedSprite::AnimatedSprite()
    : frameWidth(0), frameHeight(0), framesPerRow(3), totalRows(4),
    currentFrame(0), currentDirection(Direction::DOWN),
    animationTimer(0.0f), frameDuration(0.15f), isAnimating(false) {
}

bool AnimatedSprite::loadFromFile(const std::string& filename, int frameW, int frameH,
    int framesPerRow, int rows) {
    if (!texture.loadFromFile(filename)) {
        std::cerr << "Error: No se pudo cargar " << filename << std::endl;
        return false;
    }

    texture.setSmooth(false); // Para pixel art nítido

    frameWidth = frameW;
    frameHeight = frameH;
    this->framesPerRow = framesPerRow;
    totalRows = rows;

    sprite.setTexture(texture);
    updateTextureRect();

    return true;
}

void AnimatedSprite::updateTextureRect() {
    int row = static_cast<int>(currentDirection);
    int col = currentFrame;

    sprite.setTextureRect(sf::IntRect(
        col * frameWidth,
        row * frameHeight,
        frameWidth,
        frameHeight
    ));
}

void AnimatedSprite::setDirection(Direction dir) {
    if (currentDirection != dir) {
        currentDirection = dir;
        currentFrame = 0;
        animationTimer = 0.0f;
        updateTextureRect();
    }
}

void AnimatedSprite::play() {
    isAnimating = true;
}

void AnimatedSprite::stop() {
    isAnimating = false;
    currentFrame = 1; // Frame de reposo (frame del medio)
    updateTextureRect();
}

void AnimatedSprite::update(float deltaTime) {
    if (!isAnimating) {
        return;
    }

    animationTimer += deltaTime;

    if (animationTimer >= frameDuration) {
        animationTimer = 0.0f;
        currentFrame = (currentFrame + 1) % framesPerRow;
        updateTextureRect();
    }
}

void AnimatedSprite::setFrameDuration(float duration) {
    frameDuration = duration;
}

void AnimatedSprite::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void AnimatedSprite::setScale(float scaleX, float scaleY) {
    sprite.setScale(scaleX, scaleY);
}

sf::Vector2f AnimatedSprite::getPosition() const {
    return sprite.getPosition();
}

sf::FloatRect AnimatedSprite::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

void AnimatedSprite::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

Direction AnimatedSprite::getCurrentDirection() const {
    return currentDirection;
}

bool AnimatedSprite::getIsAnimating() const {
    return isAnimating;
}