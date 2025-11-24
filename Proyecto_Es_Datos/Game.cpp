#include "Game.h"
#include <iostream>
#include <vector>
#include <cstdlib>

Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
    "Laberinto - Estructuras de Datos"),
	score(0), running(true), gameOver(false), playerName(""),
    lastDirection(Direction::DOWN),
    isPlayerMoving(false), movementAnimDuration(0.45f),
    showMessage(false), currentMessage("") {

    if (!font.loadFromFile("assets/fonts/aria.ttf")) {
        std::cerr << "Advertencia: No se pudo cargar la fuente" << std::endl;
    }

    // Cargar sprites
    spriteManager.loadAllSprites();

    window.setFramerateLimit(60);
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            handleKeyPress(event.key.code);
        }
    }
}

void Game::handleKeyPress(sf::Keyboard::Key key) {
    char direction = ' ';
    Direction animDirection = Direction::DOWN;

    switch (key) {
    case sf::Keyboard::W:
        direction = 'W';
        animDirection = Direction::UP;
        break;
    case sf::Keyboard::S:
        direction = 'S';
        animDirection = Direction::DOWN;
        break;
    case sf::Keyboard::A:
        direction = 'A';
        animDirection = Direction::LEFT;
        break;
    case sf::Keyboard::D:
        direction = 'D';
        animDirection = Direction::RIGHT;
        break;
    case sf::Keyboard::T:
        showTreasures();
        return;
    case sf::Keyboard::X:
        useTreasure();
        return;
    default: return;
    }

    if (direction != ' ') {
        // Actualizar dirección del sprite
        lastDirection = animDirection;
        spriteManager.setPlayerDirection(animDirection);

        // Guardar tesoros antes del movimiento
        int treasuresBefore = board.getTreasuresCollected();

        if (board.movePlayer(direction)) {
            score++;

            // Verificar si se recogió un tesoro
            if (board.getTreasuresCollected() > treasuresBefore) {
                Treasure* lastTreasure = board.getLastCollectedTreasure();
                if (lastTreasure) {
                    currentMessage = "¡Encontraste un " + lastTreasure->getTypeName() + "!";
                    showMessage = true;
                    messageTimer.restart();
                }
            }
        }

        // Iniciar animación
        spriteManager.startPlayerAnimation();
        isPlayerMoving = true;
        animationClock.restart(); // Reiniciar el reloj de animación
    }
}

void Game::showTreasures() {
    board.getTreasureStack()->printStack();
}

void Game::useTreasure() {
    TreasureStack* treasures = board.getTreasureStack();

    if (treasures->isEmpty()) {
        std::cout << "No tienes tesoros para usar" << std::endl;
        return;
    }

    Treasure* treasure = treasures->pop();
    std::cout << "\n¡Usando " << treasure->getTypeName() << "!" << std::endl;

    applyTreasureEffect(treasure);

    delete treasure;
}

void Game::applyTreasureEffect(Treasure* treasure) {
    switch (treasure->getType()) {
    case TreasureType::RUBY:
        // Reducir puntos a la mitad
        score = score / 2;
        std::cout << "Puntos reducidos a la mitad. Nuevo puntaje: " << score << std::endl;
        break;

    case TreasureType::DIAMOND:
        // Eliminar 2 muros aleatorios
        removeRandomWalls(2);
        std::cout << "2 muros eliminados del tablero" << std::endl;
        break;

    case TreasureType::PEARL: {
        // 50% probabilidad: puntos x2 o puntos = 0
        int random = std::rand() % 2;
        if (random == 0) {
            score = 0;
            std::cout << "¡Oh no! Puntos reducidos a 0" << std::endl;
        }
        else {
            score *= 2;
            std::cout << "¡Suerte! Puntos duplicados: " << score << std::endl;
        }
        break;
    }

    case TreasureType::AMBER:
        // Teletransportar jugador
        teleportPlayerRandomly();
        std::cout << "¡Teletransportado a otra ubicación!" << std::endl;
        break;
    }

    // Ocultar todas las celdas después de usar un tesoro
    hideAllTiles();
}

void Game::hideAllTiles() {
    Node* currentRow = board.getHead();

    while (currentRow) {
        Node* current = currentRow;

        while (current) {
            // No ocultar paredes externas ni la posición del jugador
            if (!current->getWall() || !current->getWall()->isExternalWall()) {
                if (!current->getHasPlayer()) {
                    current->setIsRevealed(false);
                }
            }
            current = current->getRight();
        }

        currentRow = currentRow->getDown();
    }

    std::cout << "El mapa ha sido ocultado nuevamente" << std::endl;
}

void Game::removeRandomWalls(int count) {
    // Buscar todos los muros internos
    std::vector<Node*> internalWalls;

    Node* currentRow = board.getHead();
    while (currentRow) {
        Node* current = currentRow;
        while (current) {
            if (current->isWall() && !current->getWall()->isExternalWall()) {
                internalWalls.push_back(current);
            }
            current = current->getRight();
        }
        currentRow = currentRow->getDown();
    }

    // Eliminar muros aleatorios
    int removed = 0;
    while (removed < count && !internalWalls.empty()) {
        int randomIndex = std::rand() % internalWalls.size();
        Node* wallNode = internalWalls[randomIndex];

        // Eliminar el muro
        wallNode->setWall(nullptr);
        wallNode->setIsRevealed(true);

        internalWalls.erase(internalWalls.begin() + randomIndex);
        removed++;
    }
}

void Game::teleportPlayerRandomly() {
    // Buscar posiciones válidas (no muros, no tesoros, reveladas)
    std::vector<Node*> validPositions;

    Node* currentRow = board.getHead();
    while (currentRow) {
        Node* current = currentRow;
        while (current) {
            if (!current->isWall() && !current->hasTreasure() &&
                current->getIsRevealed() && !current->getHasPlayer()) {
                validPositions.push_back(current);
            }
            current = current->getRight();
        }
        currentRow = currentRow->getDown();
    }

    if (!validPositions.empty()) {
        // Remover jugador de posición actual
        Node* oldPos = board.getPlayerNode();
        if (oldPos) {
            oldPos->setHasPlayer(false);
        }

        // Colocar en nueva posición
        int randomIndex = std::rand() % validPositions.size();
        Node* newPos = validPositions[randomIndex];
        newPos->setHasPlayer(true);
    }
}

void Game::update() {
    // Obtener delta time
    float deltaTime = clock.restart().asSeconds();

    // Actualizar animación del jugador
    spriteManager.updatePlayerAnimation(deltaTime);

    // Detener animación después de un tiempo
    if (isPlayerMoving && animationClock.getElapsedTime().asSeconds() > movementAnimDuration) {
        spriteManager.stopPlayerAnimation();
        isPlayerMoving = false;
    }

    // Ocultar mensaje después de 2 segundos
    if (showMessage && messageTimer.getElapsedTime().asSeconds() > 2.0f) {
        showMessage = false;
    }

    // Verificar si ganó
    if (board.getTreasuresCollected() >= board.getTotalTreasures()) {
        std::cout << "¡Felicidades! Has encontrado todos los tesoros" << std::endl;
        std::cout << "Puntaje final: " << score << std::endl;
        // TODO: Guardar puntaje y preguntar si quiere jugar de nuevo
    }
}

void Game::render() {
    window.clear(sf::Color(40, 40, 40));

    drawBoard();
    drawUI();

    // Dibujar mensaje temporal
    if (showMessage) {
        sf::RectangleShape messageBox(sf::Vector2f(400, 60));
        messageBox.setPosition(WINDOW_WIDTH / 2 - 200, 20);
        messageBox.setFillColor(sf::Color(0, 0, 0, 180));
        messageBox.setOutlineColor(sf::Color::Yellow);
        messageBox.setOutlineThickness(3);
        window.draw(messageBox);

        sf::Text messageText;
        messageText.setFont(font);
        messageText.setString(currentMessage);
        messageText.setCharacterSize(22);
        messageText.setFillColor(sf::Color::Yellow);
        messageText.setStyle(sf::Text::Bold);

        // Centrar texto
        sf::FloatRect textBounds = messageText.getLocalBounds();
        messageText.setPosition(
            WINDOW_WIDTH / 2 - textBounds.width / 2,
            35
        );
        window.draw(messageText);
    }

    window.display();
}

void Game::drawBoard() {
    Node* currentRow = board.getHead();
    int row = 0;

    while (currentRow) {
        Node* current = currentRow;
        int col = 0;

        while (current) {
            drawCell(current, col * CELL_SIZE, row * CELL_SIZE);
            current = current->getRight();
            col++;
        }

        currentRow = currentRow->getDown();
        row++;
    }
}

void Game::drawCell(Node* node, float x, float y) {
    // Dibujar el fondo de la celda
    if (node->isWall() && node->getIsRevealed()) {
        // Dibujar muro
        if (node->getWall()->isExternalWall()) {
            spriteManager.drawWithFallback(window, SpriteID::WALL_EXTERNAL, x, y, CELL_SIZE);
        }
        else {
            spriteManager.drawWithFallback(window, SpriteID::WALL_INTERNAL, x, y, CELL_SIZE);
        }
    }
    else if (!node->getIsRevealed()) {
        // Celda oculta
        spriteManager.drawWithFallback(window, SpriteID::TILE_HIDDEN, x, y, CELL_SIZE);

        // Dibujar "o" para celdas ocultas
        sf::Text text;
        text.setFont(font);
        text.setString("o");
        text.setCharacterSize(20);
        text.setFillColor(sf::Color(150, 150, 150));
        text.setPosition(x + CELL_SIZE / 2 - 6, y + CELL_SIZE / 2 - 12);
        window.draw(text);
    }
    else {
        // Celda revelada vacía
        spriteManager.drawWithFallback(window, SpriteID::TILE_EMPTY, x, y, CELL_SIZE);
    }

    // Dibujar tesoro (si está revelado y no fue recogido)
    if (node->hasTreasure() && node->getIsRevealed()) {
        Treasure* treasure = node->getTreasure();
        SpriteID treasureSprite;

        switch (treasure->getType()) {
        case TreasureType::RUBY:
            treasureSprite = SpriteID::RUBY;
            break;
        case TreasureType::DIAMOND:
            treasureSprite = SpriteID::DIAMOND;
            break;
        case TreasureType::PEARL:
            treasureSprite = SpriteID::PEARL;
            break;
        case TreasureType::AMBER:
            treasureSprite = SpriteID::AMBER;
            break;
        }

        spriteManager.drawWithFallback(window, treasureSprite, x, y, CELL_SIZE);
    }

    // Dibujar jugador
    if (node->getHasPlayer()) {
        if (spriteManager.hasAnimatedPlayer()) {
            // Usar sprite animado
            AnimatedSprite* playerAnim = spriteManager.getPlayerSprite();
            if (playerAnim) {
                // Tu spritesheet es 124x144 con 3x4 frames
                // Cada frame es aproximadamente 41x36 píxeles
                // Queremos que ocupe aproximadamente 48x48 en pantalla
                float scaleX = 48.0f / 41.0f; // ≈ 1.17
                float scaleY = 48.0f / 36.0f; // ≈ 1.33

                playerAnim->setScale(scaleX, scaleY);
                playerAnim->setPosition(x + 6, y + 6); // Centrar en la celda
                playerAnim->draw(window);
            }
        }
        else {
            // Fallback o sprite estático
            spriteManager.drawWithFallback(window, SpriteID::PLAYER, x, y, CELL_SIZE);

            if (spriteManager.isFallbackMode()) {
                sf::Text text;
                text.setFont(font);
                text.setString("P");
                text.setCharacterSize(24);
                text.setFillColor(sf::Color::White);
                text.setPosition(x + CELL_SIZE / 3, y + CELL_SIZE / 4);
                window.draw(text);
            }
        }
    }

    // Dibujar símbolo de muro visible (solo en fallback)
    if (node->isWall() && node->getIsRevealed() && spriteManager.isFallbackMode()) {
        sf::Text text;
        text.setFont(font);
        text.setString("|");
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::White);
        text.setPosition(x + CELL_SIZE / 2 - 5, y + CELL_SIZE / 4);
        window.draw(text);
    }
}

void Game::drawUI() {
    // Puntaje
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Puntaje: " + std::to_string(score));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(20, WINDOW_HEIGHT - 120);
    window.draw(scoreText);

    // Tesoros
    sf::Text treasureText;
    treasureText.setFont(font);
    treasureText.setString("Tesoros: " + std::to_string(board.getTreasuresCollected()) +
        "/" + std::to_string(board.getTotalTreasures()));
    treasureText.setCharacterSize(24);
    treasureText.setFillColor(sf::Color::Yellow);
    treasureText.setPosition(20, WINDOW_HEIGHT - 80);
    window.draw(treasureText);

    // Controles
    sf::Text controlsText;
    controlsText.setFont(font);
    controlsText.setString("WASD: Mover | T: Ver Tesoros | X: Usar Tesoro");
    controlsText.setCharacterSize(18);
    controlsText.setFillColor(sf::Color(150, 150, 150));
    controlsText.setPosition(20, WINDOW_HEIGHT - 40);
    window.draw(controlsText);

    // Panel de tesoros recolectados
    drawTreasurePanel();
}

void Game::drawTreasurePanel() {
    TreasureStack* treasures = board.getTreasureStack();

    if (treasures->isEmpty()) {
        return;
    }

    // Fondo del panel
    sf::RectangleShape panel(sf::Vector2f(200, 150));
    panel.setPosition(WINDOW_WIDTH - 220, 20);
    panel.setFillColor(sf::Color(40, 40, 40, 200));
    panel.setOutlineColor(sf::Color(200, 200, 200));
    panel.setOutlineThickness(2);
    window.draw(panel);

    // Título
    sf::Text title;
    title.setFont(font);
    title.setString("Tesoros (T)");
    title.setCharacterSize(18);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(WINDOW_WIDTH - 210, 30);
    window.draw(title);

    // Mostrar últimos 3 tesoros
    StackNode* current = treasures->getTop();
    int yOffset = 60;
    int count = 0;

    while (current && count < 3) {
        Treasure* t = current->getTreasure();

        // Círculo de color del tesoro
        sf::CircleShape treasureCircle(10);
        treasureCircle.setFillColor(t->getColor());
        treasureCircle.setPosition(WINDOW_WIDTH - 210, yOffset);
        window.draw(treasureCircle);

        // Nombre del tesoro
        sf::Text treasureName;
        treasureName.setFont(font);
        treasureName.setString(t->getTypeName());
        treasureName.setCharacterSize(16);
        treasureName.setFillColor(sf::Color::White);
        treasureName.setPosition(WINDOW_WIDTH - 185, yOffset);
        window.draw(treasureName);

        current = current->getNext();
        yOffset += 30;
        count++;
    }

    // Indicador de más tesoros
    if (treasures->getSize() > 3) {
        sf::Text moreText;
        moreText.setFont(font);
        moreText.setString("+ " + std::to_string(treasures->getSize() - 3) + " mas...");
        moreText.setCharacterSize(14);
        moreText.setFillColor(sf::Color(150, 150, 150));
        moreText.setPosition(WINDOW_WIDTH - 210, yOffset);
        window.draw(moreText);
    }
}

void Game::drawGameOverScreen() {
    // Overlay semi-transparente
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    // Panel central
    sf::RectangleShape panel(sf::Vector2f(500, 300));
    panel.setPosition(WINDOW_WIDTH / 2 - 250, WINDOW_HEIGHT / 2 - 150);
    panel.setFillColor(sf::Color(40, 40, 40));
    panel.setOutlineColor(sf::Color(255, 215, 0)); // Dorado
    panel.setOutlineThickness(5);
    window.draw(panel);

    // Título
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("¡FELICIDADES!");
    titleText.setCharacterSize(48);
    titleText.setFillColor(sf::Color(255, 215, 0));
    titleText.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition(
        WINDOW_WIDTH / 2 - titleBounds.width / 2,
        WINDOW_HEIGHT / 2 - 120
    );
    window.draw(titleText);

    // Mensaje
    sf::Text messageText;
    messageText.setFont(font);
    messageText.setString("Has encontrado todos los tesoros");
    messageText.setCharacterSize(24);
    messageText.setFillColor(sf::Color::White);
    sf::FloatRect msgBounds = messageText.getLocalBounds();
    messageText.setPosition(
        WINDOW_WIDTH / 2 - msgBounds.width / 2,
        WINDOW_HEIGHT / 2 - 50
    );
    window.draw(messageText);

    // Puntaje final
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Puntaje Final: " + std::to_string(score));
    scoreText.setCharacterSize(32);
    scoreText.setFillColor(sf::Color(100, 255, 100));
    scoreText.setStyle(sf::Text::Bold);
    sf::FloatRect scoreBounds = scoreText.getLocalBounds();
    scoreText.setPosition(
        WINDOW_WIDTH / 2 - scoreBounds.width / 2,
        WINDOW_HEIGHT / 2 + 10
    );
    window.draw(scoreText);

    // Botón de reinicio (visual)
    sf::RectangleShape button(sf::Vector2f(300, 60));
    button.setPosition(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 + 80);
    button.setFillColor(sf::Color(70, 130, 180));
    button.setOutlineColor(sf::Color::White);
    button.setOutlineThickness(3);
    window.draw(button);

    // Texto del botón
    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString("R: Reiniciar | L: Ranking");
    buttonText.setCharacterSize(20);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setStyle(sf::Text::Bold);
    sf::FloatRect btnBounds = buttonText.getLocalBounds();
    buttonText.setPosition(
        WINDOW_WIDTH / 2 - btnBounds.width / 2,
        WINDOW_HEIGHT / 2 + 95
    );
    window.draw(buttonText);
}

void Game::resetGame() {
    // Reiniciar variables
    score = 0;
    gameOver = false;
    isPlayerMoving = false;
    showMessage = false;
    currentMessage = "";
    lastDirection = Direction::DOWN;

    // Destruir el tablero viejo y crear uno nuevo
    board.~Board();
    new (&board) Board();

    std::cout << "\n=== NUEVO JUEGO INICIADO ===" << std::endl;
}

void Game::handleTextInput(sf::Uint32 unicode) {
    // Backspace
    if (unicode == 8) {
        if (!playerName.empty()) {
            playerName.pop_back();
        }
    }
    // Caracteres válidos (letras, números, espacios)
    else if (unicode >= 32 && unicode < 127 && playerName.length() < 20) {
        playerName += static_cast<char>(unicode);
    }
}

void Game::saveScore() {
    if (!playerName.empty()) {
        scoreTree.insertPlayer(playerName, score);
        std::cout << "\nPuntaje guardado para " << playerName << ": " << score << std::endl;
    }
}

void Game::drawNameInputScreen() {
    // Overlay
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(overlay);

    // Panel
    sf::RectangleShape panel(sf::Vector2f(500, 250));
    panel.setPosition(WINDOW_WIDTH / 2 - 250, WINDOW_HEIGHT / 2 - 125);
    panel.setFillColor(sf::Color(40, 40, 40));
    panel.setOutlineColor(sf::Color(100, 150, 255));
    panel.setOutlineThickness(5);
    window.draw(panel);

    // Título
    sf::Text title;
    title.setFont(font);
    title.setString("LABERINTO DE TESOROS");
    title.setCharacterSize(32);
    title.setFillColor(sf::Color(255, 215, 0));
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(WINDOW_WIDTH / 2 - titleBounds.width / 2, WINDOW_HEIGHT / 2 - 90);
    window.draw(title);

    // Instrucción
    sf::Text instruction;
    instruction.setFont(font);
    instruction.setString("Ingresa tu nombre:");
    instruction.setCharacterSize(24);
    instruction.setFillColor(sf::Color::White);
    sf::FloatRect instBounds = instruction.getLocalBounds();
    instruction.setPosition(WINDOW_WIDTH / 2 - instBounds.width / 2, WINDOW_HEIGHT / 2 - 30);
    window.draw(instruction);

    // Campo de texto
    sf::RectangleShape textBox(sf::Vector2f(400, 50));
    textBox.setPosition(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT / 2 + 10);
    textBox.setFillColor(sf::Color(60, 60, 60));
    textBox.setOutlineColor(sf::Color::White);
    textBox.setOutlineThickness(2);
    window.draw(textBox);

    // Nombre del jugador
    sf::Text nameText;
    nameText.setFont(font);
    nameText.setString(playerName.empty() ? "_" : playerName + "_");
    nameText.setCharacterSize(28);
    nameText.setFillColor(sf::Color::White);
    nameText.setPosition(WINDOW_WIDTH / 2 - 190, WINDOW_HEIGHT / 2 + 20);
    window.draw(nameText);

    // Hint
    sf::Text hint;
    hint.setFont(font);
    hint.setString("Presiona ENTER para continuar");
    hint.setCharacterSize(18);
    hint.setFillColor(sf::Color(150, 150, 150));
    sf::FloatRect hintBounds = hint.getLocalBounds();
    hint.setPosition(WINDOW_WIDTH / 2 - hintBounds.width / 2, WINDOW_HEIGHT / 2 + 80);
    window.draw(hint);
}

void Game::drawLeaderboard() {
    std::cout << "\n===================================" << std::endl;
    std::cout << "|      TABLA DE CLASIFICACIÓN      |" << std::endl;
    std::cout << "|===================================|" << std::endl;

    std::vector<Player> topPlayers = scoreTree.getTopPlayers(10);

    if (topPlayers.empty()) {
        std::cout << "|  No hay jugadores registrados    |" << std::endl;
    }
    else {
        for (size_t i = 0; i < topPlayers.size(); i++) {
            std::cout << "| " << (i + 1) << ". ";
            std::cout << topPlayers[i].getName();

            // Espaciado
            int spaces = 25 - topPlayers[i].getName().length();
            for (int j = 0; j < spaces; j++) std::cout << " ";

            std::cout << topPlayers[i].getBestScore() << " pts |" << std::endl;
        }
    }

    std::cout << "=====================================" << std::endl;
}