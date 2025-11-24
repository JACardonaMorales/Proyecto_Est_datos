#pragma
#include "Treasure.h"
#include "Wall.h"

class Node {
private:
    bool isRevealed;
    bool isEmpty;
    Treasure* treasure;
    Wall* wall;
    bool hasPlayer;

    // Punteros para lista múltiplemente enlazada
    Node* up;
    Node* down;
    Node* left;
    Node* right;

public:
    Node();
    ~Node();

    // Getters
    bool getIsRevealed() const;
    bool getIsEmpty() const;
    bool getHasPlayer() const;
    Treasure* getTreasure() const;
    Wall* getWall() const;

    Node* getUp() const;
    Node* getDown() const;
    Node* getLeft() const;
    Node* getRight() const;

    // Setters
    void setIsRevealed(bool r);
    void setIsEmpty(bool e);
    void setHasPlayer(bool p);
    void setTreasure(Treasure* t);
    void setWall(Wall* w);

    void setUp(Node* n);
    void setDown(Node* n);
    void setLeft(Node* n);
    void setRight(Node* n);

    // Métodos útiles
    bool isWall() const;
    bool hasTreasure() const;
    void removeTreasure();
};



