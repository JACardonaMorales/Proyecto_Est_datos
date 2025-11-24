#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "SpriteManager.h"

const int BOARD_SIZE = 9;
const int CELL_SIZE = 60;
const int WINDOW_WIDTH = (BOARD_SIZE + 2) * CELL_SIZE;
const int WINDOW_HEIGHT = (BOARD_SIZE + 2) * CELL_SIZE + 150;

class Game {
private:
    sf::RenderWindow window;
    Board board;
    sf::Font font;
    SpriteManager spriteManager;
    sf::Clock clock; // Para delta time
    sf::Clock animationClock; // Para controlar duración de animación
    sf::Clock messageTimer; // Para mostrar mensajes temporales
    std::string currentMessage; // Mensaje actual a mostrar
    int score;
    bool running;
    Direction lastDirection; // Última dirección del jugador
    bool isPlayerMoving; // Si el jugador está en movimiento
    float movementAnimDuration; // Duración de la animación de movimiento
    bool showMessage; // Si hay un mensaje para mostrar
	bool gameOver;

    void handleEvents();
    void handleKeyPress(sf::Keyboard::Key key);
    void showTreasures();
    void useTreasure();
    void applyTreasureEffect(Treasure* treasure);
    void hideAllTiles();
    void removeRandomWalls(int count);
    void teleportPlayerRandomly();
    void update();
    void render();
    void drawBoard();
    void drawCell(Node* node, float x, float y);
    void drawUI();
    void drawTreasurePanel();
    void drawGameOverScreen();
    void resetGame();

public:
    Game();
    void run();
};
