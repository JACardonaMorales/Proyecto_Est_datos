#pragma once
#include <string>

class Player {
private:
    std::string name;
    int bestScore;

public:
    Player();
    Player(const std::string& n, int score);

    std::string getName() const;
    int getBestScore() const;
    void setBestScore(int score);

    // Operadores (si los tienes)
    bool operator<(const Player& other) const;
    bool operator>(const Player& other) const;
    bool operator==(const Player& other) const;
};
