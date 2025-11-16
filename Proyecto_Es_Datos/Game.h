#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"

const int BOARD_SIZE = 9;
const int CELL_SIZE = 60;
const int WINDOW_WIDTH = (BOARD_SIZE + 2) * CELL_SIZE;
const int WINDOW_HEIGHT = (BOARD_SIZE + 2) * CELL_SIZE + 150;

class Game {
private:
    sf::RenderWindow window;
    Board board;
    sf::Font font;
    int score;
    bool running;

    void handleEvents();
    void handleKeyPress(sf::Keyboard::Key key);
    void showTreasures();
    void useTreasure();
    void update();
    void render();
    void drawBoard();
    void drawCell(Node* node, float x, float y);
    void drawUI();

public:
    Game();
    void run();
};
