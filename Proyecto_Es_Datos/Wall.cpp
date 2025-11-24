#include "Wall.h"

Wall::Wall(bool external) : visible(external), isExternal(external) {}

bool Wall::isVisible() const {
    return visible;
}

void Wall::setVisible(bool v) {
    visible = v;
}

bool Wall::isExternalWall() const {
    return isExternal;
}

sf::Color Wall::getColor() const {
    if (isExternal) {
        return sf::Color(60, 60, 60); // Paredes externas más oscuras
    }
    return sf::Color(100, 100, 100); // Muros internos
}