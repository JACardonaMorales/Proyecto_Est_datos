#include "Player.h"

Player::Player() : name(""), bestScore(0) {}

Player::Player(const std::string& playerName, int score)  // Agregar const&
    : name(playerName), bestScore(score) {
}

std::string Player::getName() const {
    return name;
}

int Player::getBestScore() const {
    return bestScore;
}

void Player::setBestScore(int score) {
    bestScore = score;
}
