#include "Player.h"

Player::Player() : name(""), bestScore(0) {}

Player::Player(std::string playerName, int score)
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

// Menor puntaje es mejor
bool Player::operator<(const Player& other) const {
    return bestScore < other.bestScore;
}

bool Player::operator>(const Player& other) const {
    return bestScore > other.bestScore;
}

bool Player::operator==(const Player& other) const {
    return name == other.name;
}