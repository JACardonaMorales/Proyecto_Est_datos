#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class TreasureType {
    RUBY,      // Rubí - reduce puntos a la mitad
    DIAMOND,   // Diamante - elimina 2 muros
    PEARL,     // Perla - puntos x2 o puntos = 0
    AMBER      // Ámbar - teletransporta al jugador
};

class Treasure {
private:
    TreasureType type;
    bool collected;

public:
    Treasure(TreasureType t);

    TreasureType getType() const;
    bool isCollected() const;
    void setCollected(bool c);

    std::string getTypeName() const;
    sf::Color getColor() const;
};

