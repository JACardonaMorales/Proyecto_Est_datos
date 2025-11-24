#pragma once
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"
#include <map>
#include <string>
#include <memory>

// Enumeración para identificar los sprites
enum class SpriteID {
    PLAYER,
    RUBY,
    DIAMOND,
    PEARL,
    AMBER,
    WALL_INTERNAL,
    WALL_EXTERNAL,
    TILE_HIDDEN,
    TILE_EMPTY
};

class SpriteManager {
private:
    std::map<SpriteID, sf::Texture> textures;
    std::map<SpriteID, std::string> spritePaths;
    std::unique_ptr<AnimatedSprite> playerSprite;
    bool fallbackMode; // Si true, usa formas geométricas simples
    bool playerAnimated; // Si el jugador tiene sprite animado

    void initializePaths();
    bool loadTexture(SpriteID id);

public:
    SpriteManager();

    // Cargar todos los sprites
    bool loadAllSprites();

    // Cargar jugador animado específicamente
    bool loadAnimatedPlayer(std::string filename, int frameW, int frameH,
        int framesPerRow = 3, int rows = 4);

    // Control del jugador animado
    void updatePlayerAnimation(float deltaTime);
    void setPlayerDirection(Direction dir);
    void startPlayerAnimation();
    void stopPlayerAnimation();
    AnimatedSprite* getPlayerSprite();
    bool hasAnimatedPlayer() const;

    // Obtener una textura específica
    const sf::Texture* getTexture(SpriteID id) const;

    // Crear un sprite configurado
    sf::Sprite createSprite(SpriteID id) const;

    // Verificar si está en modo fallback
    bool isFallbackMode() const;

    // Dibujar con fallback (formas simples si no hay sprites)
    void drawWithFallback(sf::RenderWindow& window, SpriteID id,
        float x, float y, float size) const;

private:
    // Dibujar formas geométricas simples como fallback
    void drawFallbackShape(sf::RenderWindow& window, SpriteID id,
        float x, float y, float size) const;
};