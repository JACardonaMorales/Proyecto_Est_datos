#include "TreasureStack.h"
#include <iostream>

// StackNode implementation
StackNode::StackNode(Treasure* t) : treasure(t), next(nullptr) {}

StackNode::~StackNode() {
    // No eliminamos el treasure aquí porque puede ser usado
}

Treasure* StackNode::getTreasure() const {
    return treasure;
}

StackNode* StackNode::getNext() const {
    return next;
}

void StackNode::setNext(StackNode* n) {
    next = n;
}

// TreasureStack implementation
TreasureStack::TreasureStack() : top(nullptr), size(0) {}

TreasureStack::~TreasureStack() {
    clear();
}

void TreasureStack::push(Treasure* treasure) {
    StackNode* newNode = new StackNode(treasure);
    newNode->setNext(top);
    top = newNode;
    size++;
}

Treasure* TreasureStack::pop() {
    if (isEmpty()) {
        return nullptr;
    }

    StackNode* temp = top;
    Treasure* treasure = temp->getTreasure();
    top = top->getNext();

    delete temp;
    size--;

    return treasure;
}

Treasure* TreasureStack::peek() const {
    if (isEmpty()) {
        return nullptr;
    }
    return top->getTreasure();
}

bool TreasureStack::isEmpty() const {
    return top == nullptr;
}

int TreasureStack::getSize() const {
    return size;
}

void TreasureStack::clear() {
    while (!isEmpty()) {
        pop();
    }
}

void TreasureStack::printStack() const {
    if (isEmpty()) {
        std::cout << "No hay tesoros recolectados" << std::endl;
        return;
    }

    std::cout << "\n=== TESOROS RECOLECTADOS ===" << std::endl;
    StackNode* current = top;
    int position = 1;

    while (current) {
        std::cout << position << ". " << current->getTreasure()->getTypeName() << std::endl;
        current = current->getNext();
        position++;
    }
    std::cout << "Total: " << size << " tesoros" << std::endl;
}

StackNode* TreasureStack::getTop() const {
    return top;
}