#include "Board.h"
#include <cstdlib>
#include <ctime>

Board::Board() : head(nullptr), playerNode(nullptr), totalTreasures(10),
treasuresCollected(0), wallsRemaining(16) {
    std::srand(std::time(nullptr));
    initializeBoard();
}

Board::~Board() {
    // Liberar memoria de todos los nodos
    Node* currentRow = head;
    while (currentRow) {
        Node* nextRow = currentRow->getDown();
        Node* current = currentRow;

        while (current) {
            Node* next = current->getRight();
            delete current;
            current = next;
        }

        currentRow = nextRow;
    }
}

Node* Board::getNodeAt(int row, int col) {
    Node* current = head;
    for (int i = 0; i < row && current; i++) {
        current = current->getDown();
    }
    if (!current) return nullptr;

    for (int j = 0; j < col && current; j++) {
        current = current->getRight();
    }
    return current;
}

bool Board::isValidPosition(int row, int col) {
    return row > 0 && row < 10 && col > 0 && col < 10;
}

void Board::initializeBoard() {
    // Crear una cuadrícula de 11x11 (9x9 + paredes externas)
    Node* prevRow[11] = { nullptr };

    for (int i = 0; i < 11; i++) {
        Node* prevInRow = nullptr;

        for (int j = 0; j < 11; j++) {
            Node* newNode = new Node();

            // Configurar paredes exteriores
            if (i == 0 || i == 10 || j == 0 || j == 10) {
                Wall* externalWall = new Wall(true);
                newNode->setWall(externalWall);
                newNode->setIsRevealed(true);
            }
            else {
                // Interior oculto
                newNode->setIsRevealed(false);
            }

            // Enlazar horizontalmente
            if (prevInRow) {
                prevInRow->setRight(newNode);
                newNode->setLeft(prevInRow);
            }

            // Enlazar verticalmente
            if (prevRow[j]) {
                prevRow[j]->setDown(newNode);
                newNode->setUp(prevRow[j]);
            }

            prevRow[j] = newNode;
            prevInRow = newNode;

            if (i == 0 && j == 0) {
                head = newNode;
            }
        }
    }

    placeTreasures();
    placeWalls();
    placePlayer();
}

void Board::placeTreasures() {
    int placed = 0;
    while (placed < totalTreasures) {
        int row = (std::rand() % 9) + 1;
        int col = (std::rand() % 9) + 1;

        Node* node = getNodeAt(row, col);
        if (node && !node->isWall() && !node->hasTreasure()) {
            // Tipo aleatorio de tesoro
            TreasureType type = static_cast<TreasureType>(std::rand() % 4);
            Treasure* treasure = new Treasure(type);
            node->setTreasure(treasure);
            placed++;
        }
    }
}

void Board::placeWalls() {
    int placed = 0;
    while (placed < 16) {
        int row = (std::rand() % 9) + 1;
        int col = (std::rand() % 9) + 1;

        Node* node = getNodeAt(row, col);
        if (node && !node->isWall() && !node->hasTreasure()) {
            Wall* wall = new Wall(false);
            node->setWall(wall);
            placed++;
        }
    }
}

void Board::placePlayer() {
    while (true) {
        int row = (std::rand() % 9) + 1;
        int col = (std::rand() % 9) + 1;

        Node* node = getNodeAt(row, col);
        if (node && !node->isWall() && !node->hasTreasure()) {
            node->setHasPlayer(true);
            node->setIsRevealed(true);
            playerNode = node;
            break;
        }
    }
}

Node* Board::getHead() {
    return head;
}

Node* Board::getPlayerNode() {
    return playerNode;
}

void Board::setPlayerNode(Node* node) {
    playerNode = node;
}

int Board::getTreasuresCollected() const {
    return treasuresCollected;
}

int Board::getTotalTreasures() const {
    return totalTreasures;
}

bool Board::movePlayer(char direction) {
    if (!playerNode) return false;

    Node* targetNode = nullptr;
    switch (direction) {
    case 'W': targetNode = playerNode->getUp(); break;
    case 'S': targetNode = playerNode->getDown(); break;
    case 'A': targetNode = playerNode->getLeft(); break;
    case 'D': targetNode = playerNode->getRight(); break;
    }

    if (!targetNode) return false;

    // Si hay un muro, solo hacerlo visible
    if (targetNode->isWall()) {
        targetNode->setIsRevealed(true);
        return false;
    }

    // Mover jugador
    playerNode->setHasPlayer(false);
    playerNode = targetNode;
    playerNode->setHasPlayer(true);

    // Revelar celda
    if (!targetNode->getIsRevealed()) {
        targetNode->setIsRevealed(true);
    }

    // Verificar si hay tesoro
    if (targetNode->hasTreasure()) {
        Treasure* foundTreasure = targetNode->getTreasure();

        // Crear una copia del tesoro para la pila
        Treasure* treasureCopy = new Treasure(foundTreasure->getType());
        collectedTreasures.push(treasureCopy);

        targetNode->removeTreasure();
        treasuresCollected++;
    }

    return true;
}

TreasureStack* Board::getTreasureStack() {
    return &collectedTreasures;
}

Treasure* Board::getLastCollectedTreasure() {
    return collectedTreasures.peek();
}