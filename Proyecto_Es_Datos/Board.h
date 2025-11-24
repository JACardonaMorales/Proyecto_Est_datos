#pragma once
#include "Node.h"
#include "Treasure.h"
#include "TreasureStack.h"

class Board {
private:
    Node* head;
    Node* playerNode;
    int totalTreasures;
    int treasuresCollected;
    int wallsRemaining;
    TreasureStack collectedTreasures;

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
    TreasureStack* getTreasureStack();

    bool movePlayer(char direction);
    Treasure* getLastCollectedTreasure();
};
