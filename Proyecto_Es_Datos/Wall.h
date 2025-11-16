#pragma once
#include <SFML/Graphics.hpp>

class Wall {
private:
    bool visible;
    bool isExternal;

public:
    Wall(bool external = false);

    bool isVisible() const;
    void setVisible(bool v);

    bool isExternalWall() const;
    sf::Color getColor() const;
};
