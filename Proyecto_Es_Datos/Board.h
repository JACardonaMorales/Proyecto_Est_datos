#pragma once
#include "Nodo.h"

class Board {
private:
    Node* head;
    Node* playerNode;
    int totalTreasures;
    int treasuresCollected;
    int wallsRemaining;

    // Helper privados
    Node* getNodeAt(int row, int col);
    bool isValidPosition(int row, int col);

public:
    Board();
    ~Board();

    void initializeBoard();
    void placeTreasures();
    void placeWalls();
    void placePlayer();

    Node* getHead();
    Node* getPlayerNode();
    int getTreasuresCollected() const;
    int getTotalTreasures() const;

    bool movePlayer(char direction);
};

