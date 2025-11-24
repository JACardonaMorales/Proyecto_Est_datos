#pragma once
#include "Player.h"
#include <vector>
#include <memory>

// Nodo del árbol
class TreeNode {
private:
    Player player;
    TreeNode* left;
    TreeNode* right;

public:
    TreeNode(Player p);
    ~TreeNode();

    Player& getPlayer();
    TreeNode* getLeft() const;
    TreeNode* getRight() const;
    void setLeft(TreeNode* node);
    void setRight(TreeNode* node);
};

// Árbol Binario de Búsqueda para puntajes
class ScoreTree {
private:
    TreeNode* root;
    int totalPlayers;

    // Métodos privados recursivos
    TreeNode* insertNode(TreeNode* node, Player player);
    TreeNode* searchNode(TreeNode* node, std::string name);
    void inOrderTraversal(TreeNode* node, std::vector<Player>& players);
    void destroyTree(TreeNode* node);

public:
    ScoreTree();
    ~ScoreTree();

    // Insertar o actualizar jugador
    void insertPlayer(std::string name, int score);

    // Buscar jugador por nombre
    Player* findPlayer(std::string name);

    // Obtener todos los jugadores ordenados por puntaje (menor a mayor)
    std::vector<Player> getAllPlayersSorted();

    // Obtener top N jugadores
    std::vector<Player> getTopPlayers(int n);

    int getTotalPlayers() const;
};