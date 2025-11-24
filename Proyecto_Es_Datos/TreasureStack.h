#pragma once
#include "Treasure.h"
#include <memory>

// Nodo de la pila
class StackNode {
private:
    Treasure* treasure;
    StackNode* next;

public:
    StackNode(Treasure* t);
    ~StackNode();

    Treasure* getTreasure() const;
    StackNode* getNext() const;
    void setNext(StackNode* n);
};

// Pila de tesoros (LIFO - Last In First Out)
class TreasureStack {
private:
    StackNode* top;
    int size;

public:
    TreasureStack();
    ~TreasureStack();

    // Operaciones de pila
    void push(Treasure* treasure);
    Treasure* pop();
    Treasure* peek() const;

    // Utilidades
    bool isEmpty() const;
    int getSize() const;
    void clear();

    // Para mostrar todos los tesoros
    void printStack() const;
    StackNode* getTop() const;
};