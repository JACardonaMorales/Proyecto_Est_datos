#pragma once
#include <string>

class Player {
private:
    std::string name;
    int bestScore;

public:
    Player() : name(""), bestScore(0) {}
    Player(const std::string& n, int score) : name(n), bestScore(score) {}

    std::string getName() const { return name; }
    int getBestScore() const { return bestScore; }
    void setBestScore(int score) { bestScore = score; }
};
