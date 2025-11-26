#include "ScoreTree.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;

ScoreTree::ScoreTree() : root(nullptr), jsonFilePath("scores.json") {
    loadFromJson();
}

ScoreTree::ScoreTree(const std::string& jsonPath) : root(nullptr), jsonFilePath(jsonPath) {
    loadFromJson();
}

ScoreTree::~ScoreTree() {
    saveToJson();
    clear(root);
}

void ScoreTree::clear(TreeNode* node) {
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

ScoreTree::TreeNode* ScoreTree::insert(TreeNode* node, const Player& player) {
    if (node == nullptr) {
        return new TreeNode(player);
    }

    // Ordenar por score (menor es mejor)
    if (player.getBestScore() < node->player.getBestScore()) {
        node->left = insert(node->left, player);
    }
    else if (player.getBestScore() > node->player.getBestScore()) {
        node->right = insert(node->right, player);
    }
    else {
        // Si los scores son iguales, ordenar alfabéticamente por nombre
        if (player.getName() < node->player.getName()) {
            node->left = insert(node->left, player);
        }
        else {
            node->right = insert(node->right, player);
        }
    }

    return node;
}

void ScoreTree::insertPlayer(const std::string& name, int score) {
    // Verificar si el jugador ya existe
    Player* existingPlayer = findPlayer(name);

    if (existingPlayer != nullptr) {
        // Solo actualizar si el nuevo puntaje es MEJOR (menor)
        if (score < existingPlayer->getBestScore()) {
            std::cout << "Nuevo récord para " << name << "! Anterior: "
                << existingPlayer->getBestScore() << " -> Nuevo: " << score << std::endl;

            // Eliminar el nodo viejo y reinsertar con nuevo score
            // (Implementación simplificada: reconstruir árbol)
            std::vector<Player> allPlayers;
            serializeTree(root, allPlayers);

            // Limpiar árbol actual
            clear(root);
            root = nullptr;

            // Reinsertar todos excepto el jugador a actualizar
            for (const auto& p : allPlayers) {
                if (p.getName() != name) {
                    root = insert(root, p);
                }
            }

            // Insertar con nuevo score
            Player updatedPlayer(name, score);
            root = insert(root, updatedPlayer);
        }
        else {
            std::cout << "El puntaje anterior de " << name << " ("
                << existingPlayer->getBestScore() << ") es mejor. No se actualiza." << std::endl;
        }
    }
    else {
        // Jugador nuevo
        Player newPlayer(name, score);
        root = insert(root, newPlayer);
        std::cout << "Jugador " << name << " registrado con " << score << " puntos." << std::endl;
    }

    // Guardar automáticamente después de cada inserción
    saveToJson();
}

ScoreTree::TreeNode* ScoreTree::search(TreeNode* node, const std::string& name) const {
    if (node == nullptr) {
        return nullptr;
    }

    if (node->player.getName() == name) {
        return node;
    }

    // Buscar en ambos subárboles
    TreeNode* leftResult = search(node->left, name);
    if (leftResult != nullptr) {
        return leftResult;
    }

    return search(node->right, name);
}

Player* ScoreTree::findPlayer(const std::string& name) const {
    TreeNode* node = search(root, name);
    if (node != nullptr) {
        return &(node->player);
    }
    return nullptr;
}

void ScoreTree::inorderReverse(TreeNode* node, std::vector<Player>& players) const {
    if (node == nullptr) return;

    // Inorden invertido: derecha -> raíz -> izquierda
    // (para obtener de mayor a menor, pero nosotros queremos menor a mayor)
    // Entonces usamos inorden normal: izquierda -> raíz -> derecha
    inorderReverse(node->left, players);
    players.push_back(node->player);
    inorderReverse(node->right, players);
}

void ScoreTree::serializeTree(TreeNode* node, std::vector<Player>& players) const {
    if (node == nullptr) return;

    players.push_back(node->player);
    serializeTree(node->left, players);
    serializeTree(node->right, players);
}

std::vector<Player> ScoreTree::getTopPlayers(int count) const {
    std::vector<Player> allPlayers;
    inorderReverse(root, allPlayers);

    if (allPlayers.size() > count) {
        allPlayers.resize(count);
    }

    return allPlayers;
}

void ScoreTree::printAllPlayers() const {
    std::vector<Player> players;
    inorderReverse(root, players);

    std::cout << "\n===== TODOS LOS JUGADORES =====" << std::endl;
    if (players.empty()) {
        std::cout << "No hay jugadores registrados." << std::endl;
    }
    else {
        for (size_t i = 0; i < players.size(); i++) {
            std::cout << (i + 1) << ". " << players[i].getName()
                << " - " << players[i].getBestScore() << " puntos" << std::endl;
        }
    }
    std::cout << "===============================" << std::endl;
}

void ScoreTree::saveToJson() const {
    json j;
    std::vector<Player> players;

    // Serializar todos los jugadores
    serializeTree(root, players);

    // Convertir a JSON
    for (const auto& player : players) {
        j.push_back({
            {"name", player.getName()},
            {"bestScore", player.getBestScore()}
            });
    }

    // Escribir al archivo
    std::ofstream file(jsonFilePath);
    if (file.is_open()) {
        file << j.dump(4); // dump(4) formatea con indentación
        file.close();
        std::cout << "[GUARDADO] Puntajes guardados en " << jsonFilePath << std::endl;
    }
    else {
        std::cerr << "[ERROR] No se pudo guardar " << jsonFilePath << std::endl;
    }
}

void ScoreTree::loadFromJson() {
    std::ifstream file(jsonFilePath);

    if (!file.is_open()) {
        std::cout << "[INFO] No se encontró " << jsonFilePath << ". Se creará uno nuevo." << std::endl;
        return;
    }

    try {
        json j;
        file >> j;
        file.close();

        // Cargar cada jugador al árbol
        for (const auto& item : j) {
            std::string name = item["name"];
            int score = item["bestScore"];

            Player player(name, score);
            root = insert(root, player);
        }

        std::cout << "[CARGADO] " << j.size() << " jugador(es) cargados desde " << jsonFilePath << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "[ERROR] Error al cargar JSON: " << e.what() << std::endl;
    }
}

void ScoreTree::save() {
    saveToJson();
}

void ScoreTree::load() {
    loadFromJson();
}
