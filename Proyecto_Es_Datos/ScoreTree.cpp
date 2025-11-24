#include "ScoreTree.h"
#include <iostream>
#include <algorithm>

// TreeNode implementation
TreeNode::TreeNode(Player p) : player(p), left(nullptr), right(nullptr) {}

TreeNode::~TreeNode() {
    // Los nodos hijos se eliminan en destroyTree
}

Player& TreeNode::getPlayer() {
    return player;
}

TreeNode* TreeNode::getLeft() const {
    return left;
}

TreeNode* TreeNode::getRight() const {
    return right;
}

void TreeNode::setLeft(TreeNode* node) {
    left = node;
}

void TreeNode::setRight(TreeNode* node) {
    right = node;
}

// ScoreTree implementation
ScoreTree::ScoreTree() : root(nullptr), totalPlayers(0) {}

ScoreTree::~ScoreTree() {
    destroyTree(root);
}

void ScoreTree::destroyTree(TreeNode* node) {
    if (node) {
        destroyTree(node->getLeft());
        destroyTree(node->getRight());
        delete node;
    }
}

TreeNode* ScoreTree::insertNode(TreeNode* node, Player player) {
    if (!node) {
        totalPlayers++;
        return new TreeNode(player);
    }

    // Si el jugador ya existe, actualizar solo si el nuevo puntaje es mejor
    if (player.getName() == node->getPlayer().getName()) {
        if (player.getBestScore() < node->getPlayer().getBestScore()) {
            node->getPlayer().setBestScore(player.getBestScore());
            std::cout << "Nuevo mejor puntaje para " << player.getName()
                << ": " << player.getBestScore() << std::endl;
        }
        else {
            std::cout << "Puntaje anterior de " << player.getName()
                << " era mejor: " << node->getPlayer().getBestScore() << std::endl;
        }
        return node;
    }

    // Insertar según el nombre (orden alfabético)
    if (player.getName() < node->getPlayer().getName()) {
        node->setLeft(insertNode(node->getLeft(), player));
    }
    else {
        node->setRight(insertNode(node->getRight(), player));
    }

    return node;
}

void ScoreTree::insertPlayer(std::string name, int score) {
    Player newPlayer(name, score);
    root = insertNode(root, newPlayer);
}

TreeNode* ScoreTree::searchNode(TreeNode* node, std::string name) {
    if (!node) {
        return nullptr;
    }

    if (name == node->getPlayer().getName()) {
        return node;
    }

    if (name < node->getPlayer().getName()) {
        return searchNode(node->getLeft(), name);
    }
    else {
        return searchNode(node->getRight(), name);
    }
}

Player* ScoreTree::findPlayer(std::string name) {
    TreeNode* node = searchNode(root, name);
    if (node) {
        return &(node->getPlayer());
    }
    return nullptr;
}

void ScoreTree::inOrderTraversal(TreeNode* node, std::vector<Player>& players) {
    if (node) {
        inOrderTraversal(node->getLeft(), players);
        players.push_back(node->getPlayer());
        inOrderTraversal(node->getRight(), players);
    }
}

std::vector<Player> ScoreTree::getAllPlayersSorted() {
    std::vector<Player> players;
    inOrderTraversal(root, players);

    // Ordenar por puntaje (menor es mejor)
    std::sort(players.begin(), players.end(),
        [](const Player& a, const Player& b) {
            return a.getBestScore() < b.getBestScore();
        });

    return players;
}

std::vector<Player> ScoreTree::getTopPlayers(int n) {
    std::vector<Player> allPlayers = getAllPlayersSorted();

    if (allPlayers.size() <= n) {
        return allPlayers;
    }

    return std::vector<Player>(allPlayers.begin(), allPlayers.begin() + n);
}

int ScoreTree::getTotalPlayers() const {
    return totalPlayers;
}