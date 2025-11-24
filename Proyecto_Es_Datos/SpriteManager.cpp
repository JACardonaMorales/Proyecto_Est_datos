#include "SpriteManager.h"
#include <iostream>

SpriteManager::SpriteManager() : fallbackMode(false), playerAnimated(false) {
    initializePaths();
    playerSprite = std::make_unique<AnimatedSprite>();
}

void SpriteManager::initializePaths() {
    // Definir rutas de los sprites
    spritePaths[SpriteID::PLAYER] = "assets/sprites/player.png";
    spritePaths[SpriteID::RUBY] = "assets/sprites/treasures/ruby.png";
    spritePaths[SpriteID::DIAMOND] = "assets/sprites/treasures/diamond.png";
    spritePaths[SpriteID::PEARL] = "assets/sprites/treasures/pearl.png";
    spritePaths[SpriteID::AMBER] = "assets/sprites/treasures/amber.png";
    spritePaths[SpriteID::WALL_INTERNAL] = "assets/sprites/walls/wall_internal.png";
    spritePaths[SpriteID::WALL_EXTERNAL] = "assets/sprites/walls/wall_external.png";
    spritePaths[SpriteID::TILE_HIDDEN] = "assets/sprites/tiles/tile_hidden.png";
    spritePaths[SpriteID::TILE_EMPTY] = "assets/sprites/tiles/tile_empty.png";
}

bool SpriteManager::loadTexture(SpriteID id) {
    auto it = spritePaths.find(id);
    if (it == spritePaths.end()) {
        return false;
    }

    sf::Texture texture;
    if (!texture.loadFromFile(it->second)) {
        std::cerr << "Advertencia: No se pudo cargar " << it->second << std::endl;
        return false;
    }

    texture.setSmooth(false); // Para pixel nítido
    textures[id] = texture;
    return true;
}

bool SpriteManager::loadAllSprites() {
    bool allLoaded = true;
    int loadedCount = 0;
    int totalSprites = spritePaths.size();

    // Intentar cargar jugador animado primero
    // 124x144 spritesheet = 3 columnas x 4 filas
    // Frame size: 124/3 ≈ 41.33, pero debería ser exacto
    // Probemos con dimensiones reales
    if (loadAnimatedPlayer("assets/sprites/player.png", 41, 36, 3, 4)) {
        std::cout << "Jugador animado cargado correctamente" << std::endl;
        playerAnimated = true;
        loadedCount++;
    }
    else {
        // Si falla, intentar cargar como sprite estático
        if (loadTexture(SpriteID::PLAYER)) {
            loadedCount++;
        }
        else {
            allLoaded = false;
        }
    }

    // Cargar resto de sprites
    for (const auto& pair : spritePaths) {
        if (pair.first == SpriteID::PLAYER) continue; // Ya lo intentamos

        if (loadTexture(pair.first)) {
            loadedCount++;
        }
        else {
            allLoaded = false;
        }
    }

    std::cout << "Sprites cargados: " << loadedCount << "/" << totalSprites << std::endl;

    // Si no se cargó ningún sprite, activar modo fallback
    if (loadedCount == 0) {
        fallbackMode = true;
        std::cout << "Usando modo fallback (formas geometricas)" << std::endl;
    }

    return allLoaded;
}

const sf::Texture* SpriteManager::getTexture(SpriteID id) const {
    auto it = textures.find(id);
    if (it != textures.end()) {
        return &(it->second);
    }
    return nullptr;
}

sf::Sprite SpriteManager::createSprite(SpriteID id) const {
    sf::Sprite sprite;
    const sf::Texture* texture = getTexture(id);

    if (texture) {
        sprite.setTexture(*texture);

        // Escalar para ajustar al tamaño de celda si es necesario
        sf::Vector2u textureSize = texture->getSize();

        // Para tesoros (más pequeños), centrar en la celda
        if (id == SpriteID::RUBY || id == SpriteID::DIAMOND ||
            id == SpriteID::PEARL || id == SpriteID::AMBER) {
            // Los tesoros se escalan a 32x32 y se centran
            float scale = 32.0f / textureSize.x;
            sprite.setScale(scale, scale);
        }
        // Para jugador, escalar a 48x48
        else if (id == SpriteID::PLAYER) {
            float scale = 48.0f / textureSize.x;
            sprite.setScale(scale, scale);
        }
        // Para tiles y paredes, escalar a 60x60
        else {
            float scaleX = 60.0f / textureSize.x;
            float scaleY = 60.0f / textureSize.y;
            sprite.setScale(scaleX, scaleY);
        }
    }

    return sprite;
}

bool SpriteManager::isFallbackMode() const {
    return fallbackMode;
}

void SpriteManager::drawWithFallback(sf::RenderWindow& window, SpriteID id,
    float x, float y, float size) const {
    const sf::Texture* texture = getTexture(id);

    if (texture) {
        // Dibujar sprite normal
        sf::Sprite sprite = createSprite(id);

        // Ajustar posición según el tipo
        if (id == SpriteID::RUBY || id == SpriteID::DIAMOND ||
            id == SpriteID::PEARL || id == SpriteID::AMBER) {
            // Centrar tesoros (32x32 en celda de 60x60)
            sprite.setPosition(x + 14, y + 14);
        }
        else if (id == SpriteID::PLAYER) {
            // Centrar jugador (48x48 en celda de 60x60)
            sprite.setPosition(x + 6, y + 6);
        }
        else {
            // Tiles y paredes ocupan toda la celda
            sprite.setPosition(x, y);
        }

        window.draw(sprite);
    }
    else {
        // Modo fallback: dibujar formas geométricas simples
        drawFallbackShape(window, id, x, y, size);
    }
}

void SpriteManager::drawFallbackShape(sf::RenderWindow& window, SpriteID id,
    float x, float y, float size) const {
    switch (id) {
    case SpriteID::PLAYER: {
        sf::CircleShape player(size / 3);
        player.setFillColor(sf::Color::Green);
        player.setPosition(x + size / 6, y + size / 6);
        window.draw(player);
        break;
    }

    case SpriteID::RUBY: {
        sf::CircleShape ruby(size / 4);
        ruby.setFillColor(sf::Color::Red);
        ruby.setPosition(x + size / 4, y + size / 4);
        window.draw(ruby);
        break;
    }

    case SpriteID::DIAMOND: {
        sf::CircleShape diamond(size / 4);
        diamond.setFillColor(sf::Color::Cyan);
        diamond.setPosition(x + size / 4, y + size / 4);
        window.draw(diamond);
        break;
    }

    case SpriteID::PEARL: {
        sf::CircleShape pearl(size / 4);
        pearl.setFillColor(sf::Color(255, 192, 203));
        pearl.setPosition(x + size / 4, y + size / 4);
        window.draw(pearl);
        break;
    }

    case SpriteID::AMBER: {
        sf::CircleShape amber(size / 4);
        amber.setFillColor(sf::Color(255, 191, 0));
        amber.setPosition(x + size / 4, y + size / 4);
        window.draw(amber);
        break;
    }

    case SpriteID::WALL_INTERNAL: {
        sf::RectangleShape wall(sf::Vector2f(size - 2, size - 2));
        wall.setFillColor(sf::Color(100, 100, 100));
        wall.setPosition(x + 1, y + 1);
        window.draw(wall);
        break;
    }

    case SpriteID::WALL_EXTERNAL: {
        sf::RectangleShape wall(sf::Vector2f(size - 2, size - 2));
        wall.setFillColor(sf::Color(60, 60, 60));
        wall.setPosition(x + 1, y + 1);
        window.draw(wall);
        break;
    }

    case SpriteID::TILE_HIDDEN: {
        sf::RectangleShape tile(sf::Vector2f(size - 2, size - 2));
        tile.setFillColor(sf::Color(100, 100, 100));
        tile.setPosition(x + 1, y + 1);
        window.draw(tile);
        break;
    }

    case SpriteID::TILE_EMPTY: {
        sf::RectangleShape tile(sf::Vector2f(size - 2, size - 2));
        tile.setFillColor(sf::Color(200, 200, 200));
        tile.setPosition(x + 1, y + 1);
        window.draw(tile);
        break;
    }
    }
}

bool SpriteManager::loadAnimatedPlayer(std::string filename, int frameW, int frameH,
    int framesPerRow, int rows) {
    if (playerSprite->loadFromFile(filename, frameW, frameH, framesPerRow, rows)) {
        // Configurar velocidad de animación (más lento = más realista)
        playerSprite->setFrameDuration(0.90f); // 150ms por frame
        playerAnimated = true;
        return true;
    }
    return false;
}

void SpriteManager::updatePlayerAnimation(float deltaTime) {
    if (playerAnimated && playerSprite) {
        playerSprite->update(deltaTime);
    }
}

void SpriteManager::setPlayerDirection(Direction dir) {
    if (playerAnimated && playerSprite) {
        playerSprite->setDirection(dir);
    }
}

void SpriteManager::startPlayerAnimation() {
    if (playerAnimated && playerSprite) {
        playerSprite->play();
    }
}

void SpriteManager::stopPlayerAnimation() {
    if (playerAnimated && playerSprite) {
        playerSprite->stop();
    }
}

AnimatedSprite* SpriteManager::getPlayerSprite() {
    return playerSprite.get();
}

bool SpriteManager::hasAnimatedPlayer() const {
    return playerAnimated;
}