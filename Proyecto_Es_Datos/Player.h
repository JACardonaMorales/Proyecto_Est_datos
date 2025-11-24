#pragma once
#include <string>

class Player {
private:
    std::string name;
    int bestScore;

public:
    Player();
    Player(std::string playerName, int score);

    std::string getName() const;
    int getBestScore() const;
    void setBestScore(int score);

    // Para comparación en el ABB (menor puntaje = mejor)
    bool operator<(const Player& other) const;
    bool operator>(const Player& other) const;
    bool operator==(const Player& other) const;
};