#include "Node.h"

Node::Node() : isRevealed(false), isEmpty(false), treasure(nullptr),
wall(nullptr), hasPlayer(false),
up(nullptr), down(nullptr), left(nullptr), right(nullptr) {
}

Node::~Node() {
    if (treasure) delete treasure;
    if (wall) delete wall;
}

// Getters
bool Node::getIsRevealed() const {
    return isRevealed;
}

bool Node::getIsEmpty() const {
    return isEmpty;
}

bool Node::getHasPlayer() const {
    return hasPlayer;
}

Treasure* Node::getTreasure() const {
    return treasure;
}

Wall* Node::getWall() const {
    return wall;
}

Node* Node::getUp() const {
    return up;
}

Node* Node::getDown() const {
    return down;
}

Node* Node::getLeft() const {
    return left;
}

Node* Node::getRight() const {
    return right;
}

// Setters
void Node::setIsRevealed(bool r) {
    isRevealed = r;
}

void Node::setIsEmpty(bool e) {
    isEmpty = e;
}

void Node::setHasPlayer(bool p) {
    hasPlayer = p;
}

void Node::setTreasure(Treasure* t) {
    if (treasure) delete treasure;
    treasure = t;
}

void Node::setWall(Wall* w) {
    if (wall) delete wall;
    wall = w;
}

void Node::setUp(Node* n) {
    up = n;
}

void Node::setDown(Node* n) {
    down = n;
}

void Node::setLeft(Node* n) {
    left = n;
}

void Node::setRight(Node* n) {
    right = n;
}

// Métodos útiles
bool Node::isWall() const {
    return wall != nullptr;
}

bool Node::hasTreasure() const {
    return treasure != nullptr && !treasure->isCollected();
}

void Node::removeTreasure() {
    if (treasure) {
        treasure->setCollected(true);
    }
}

