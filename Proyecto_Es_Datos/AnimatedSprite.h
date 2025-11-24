#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

enum class Direction {
    DOWN = 0,   // Fila 0: mirando abajo ✓
    LEFT = 3,   // Fila 1: mirando izquierda (era derecha antes)
    RIGHT = 1,  // Fila 2: mirando derecha (era izquierda antes)
    UP = 2      // Fila 3: mirando arriba (era izquierda antes)
};

class AnimatedSprite {
private:
    sf::Texture texture;
    sf::Sprite sprite;

    // Configuración del spritesheet
    int frameWidth;      // Ancho de cada frame
    int frameHeight;     // Alto de cada frame
    int framesPerRow;    // Frames por fila (animación)
    int totalRows;       // Filas totales (direcciones)

    // Estado de animación
    int currentFrame;
    Direction currentDirection;
    float animationTimer;
    float frameDuration;  // Tiempo por frame en segundos
    bool isAnimating;

    void updateTextureRect();

public:
    AnimatedSprite();

    // Cargar spritesheet
    bool loadFromFile(const std::string& filename, int frameW, int frameH,
        int framesPerRow = 3, int rows = 4);

    // Control de animación
    void setDirection(Direction dir);
    void play();
    void stop();
    void update(float deltaTime);

    // Control de velocidad
    void setFrameDuration(float duration);

    // Dibujo
    void setPosition(float x, float y);
    void setScale(float scaleX, float scaleY);
    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;

    void draw(sf::RenderWindow& window);

    // Getters
    Direction getCurrentDirection() const;
    bool getIsAnimating() const;
};