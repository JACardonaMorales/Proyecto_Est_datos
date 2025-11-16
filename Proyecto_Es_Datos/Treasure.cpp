#include "Treasure.h"

Treasure::Treasure(TreasureType t) : type(t), collected(false) {}

TreasureType Treasure::getType() const {
    return type;
}

bool Treasure::isCollected() const {
    return collected;
}

void Treasure::setCollected(bool c) {
    collected = c;
}

std::string Treasure::getTypeName() const {
    switch (type) {
    case TreasureType::RUBY: return "Rubi";
    case TreasureType::DIAMOND: return "Diamante";
    case TreasureType::PEARL: return "Perla";
    case TreasureType::AMBER: return "Ambar";
    default: return "Desconocido";
    }
}

sf::Color Treasure::getColor() const {
    switch (type) {
    case TreasureType::RUBY: return sf::Color::Red;
    case TreasureType::DIAMOND: return sf::Color::Cyan;
    case TreasureType::PEARL: return sf::Color(255, 192, 203); // Rosa
    case TreasureType::AMBER: return sf::Color(255, 191, 0); // Naranja
    default: return sf::Color::Yellow;
    }
}
