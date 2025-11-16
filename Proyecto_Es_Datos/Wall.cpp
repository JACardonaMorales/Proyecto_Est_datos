#include "Game.h"
#include <iostream>

Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
    "Laberinto - Estructuras de Datos"),
    score(0), running(true) {

    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Advertencia: No se pudo cargar la fuente" << std::endl;
    }

    window.setFramerateLimit(60);
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            handleKeyPress(event.key.code);
        }
    }
}

void Game::handleKeyPress(sf::Keyboard::Key key) {
    char direction = ' ';

    switch (key) {
    case sf::Keyboard::W: direction = 'W'; break;
    case sf::Keyboard::S: direction = 'S'; break;
    case sf::Keyboard::A: direction = 'A'; break;
    case sf::Keyboard::D: direction = 'D'; break;
    case sf::Keyboard::T:
        showTreasures();
        return;
    case sf::Keyboard::X:
        useTreasure();
        return;
    default: return;
    }

    if (direction != ' ') {
        if (board.movePlayer(direction)) {
            score++;
        }
    }
}

void Game::showTreasures() {
    // TODO: Mostrar pila de tesoros
    std::cout << "Tesoros recolectados: " << board.getTreasuresCollected() << std::endl;
}

void Game::useTreasure() {
    // TODO: Usar último tesoro de la pila
    std::cout << "Usando tesoro..." << std::endl;
}

void Game::update() {
    // Verificar si ganó
    if (board.getTreasuresCollected() >= board.getTotalTreasures()) {
        std::cout << "¡Felicidades! Has encontrado todos los tesoros" << std::endl;
        std::cout << "Puntaje final: " << score << std::endl;
        // TODO: Guardar puntaje y preguntar si quiere jugar de nuevo
    }
}

void Game::render() {
    window.clear(sf::Color(40, 40, 40));

    drawBoard();
    drawUI();

    window.display();
}

void Game::drawBoard() {
    Node* currentRow = board.getHead();
    int row = 0;

    while (currentRow) {
        Node* current = currentRow;
        int col = 0;

        while (current) {
            drawCell(current, col * CELL_SIZE, row * CELL_SIZE);
            current = current->getRight();
            col++;
        }

        currentRow = currentRow->getDown();
        row++;
    }
}

void Game::drawCell(Node* node, float x, float y) {
    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
    cell.setPosition(x + 1, y + 1);
    cell.setOutlineThickness(1);
    cell.setOutlineColor(sf::Color(50, 50, 50));

    // Determinar color de la celda
    if (node->isWall() && node->getIsRevealed()) {
        cell.setFillColor(node->getWall()->getColor());
    }
    else if (!node->getIsRevealed()) {
        cell.setFillColor(sf::Color(100, 100, 100));

        // Dibujar "o" para celdas ocultas
        sf::Text text;
        text.setFont(font);
        text.setString("o");
        text.setCharacterSize(20);
        text.setFillColor(sf::Color(150, 150, 150));
        text.setPosition(x + CELL_SIZE / 2 - 6, y + CELL_SIZE / 2 - 12);
        window.draw(cell);
        window.draw(text);
        return;
    }
    else {
        cell.setFillColor(sf::Color(200, 200, 200));
    }

    window.draw(cell);

    // Dibujar jugador
    if (node->getHasPlayer()) {
        sf::CircleShape player(CELL_SIZE / 3);
        player.setFillColor(sf::Color::Green);
        player.setPosition(x + CELL_SIZE / 6, y + CELL_SIZE / 6);
        window.draw(player);

        sf::Text text;
        text.setFont(font);
        text.setString("P");
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition(x + CELL_SIZE / 3, y + CELL_SIZE / 4);
        window.draw(text);
    }

    // Dibujar tesoro (si está revelado y no fue recogido)
    if (node->hasTreasure() && node->getIsRevealed()) {
        sf::CircleShape treasureShape(CELL_SIZE / 4);
        treasureShape.setFillColor(node->getTreasure()->getColor());
        treasureShape.setPosition(x + CELL_SIZE / 4, y + CELL_SIZE / 4);
        window.draw(treasureShape);
    }

    // Dibujar símbolo de muro visible
    if (node->isWall() && node->getIsRevealed()) {
        sf::Text text;
        text.setFont(font);
        text.setString("|");
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::White);
        text.setPosition(x + CELL_SIZE / 2 - 5, y + CELL_SIZE / 4);
        window.draw(text);
    }
}

void Game::drawUI() {
    // Puntaje
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Puntaje: " + std::to_string(score));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(20, WINDOW_HEIGHT - 120);
    window.draw(scoreText);

    // Tesoros
    sf::Text treasureText;
    treasureText.setFont(font);
    treasureText.setString("Tesoros: " + std::to_string(board.getTreasuresCollected()) +
        "/" + std::to_string(board.getTotalTreasures()));
    treasureText.setCharacterSize(24);
    treasureText.setFillColor(sf::Color::Yellow);
    treasureText.setPosition(20, WINDOW_HEIGHT - 80);
    window.draw(treasureText);

    // Controles
    sf::Text controlsText;
    controlsText.setFont(font);
    controlsText.setString("WASD: Mover | T: Ver Tesoros | X: Usar Tesoro");
    controlsText.setCharacterSize(18);
    controlsText.setFillColor(sf::Color(150, 150, 150));
    controlsText.setPosition(20, WINDOW_HEIGHT - 40);
    window.draw(controlsText);
}
