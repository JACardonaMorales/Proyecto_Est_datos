#pragma once
#include "Player.h"
#include <vector>
#include <string>

class ScoreTree {
private:
    struct TreeNode {
        Player player;
        TreeNode* left;
        TreeNode* right;

        TreeNode(const Player& p) : player(p), left(nullptr), right(nullptr) {}
    };

    TreeNode* root;
    std::string jsonFilePath;

    // Metodos privados auxiliares
    TreeNode* insert(TreeNode* node, const Player& player);
    void inorderReverse(TreeNode* node, std::vector<Player>& players) const;
    void clear(TreeNode* node);
    TreeNode* search(TreeNode* node, const std::string& name) const;

    // Nuevos metodos para JSON
    void saveToJson() const;
    void loadFromJson();
    void serializeTree(TreeNode* node, std::vector<Player>& players) const;

public:
    ScoreTree();
    ScoreTree(const std::string& jsonPath);
    ~ScoreTree();

    void insertPlayer(const std::string& name, int score);
    std::vector<Player> getTopPlayers(int count) const;
    Player* findPlayer(const std::string& name) const;
    void printAllPlayers() const;

    // Metodos de persistencia (nuevoo)
    void save();
    void load();
};
