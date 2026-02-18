/* 
*   File: game.cpp
*   Name: Nick Cully
*   Version: Part 3 (Pointers and Dynamic Memory)
*   Version: Part 4 
*   Date: 02/11/2026
*   Class: CS - 162 - (02)
*   Description: Assignment 3.2, Final part of the Asteroids clone. Creates an array of finite torpedos with pointers, torpedos have a lifetime, torpedos delete after collisions, and make the asteroids split for full credit.
*
*/

#include <SFML/Graphics.hpp>
#include "spaceObject.h"
#include "constants.h"
#include <optional>
#include <cmath>
#include <cstdlib>
#include <iostream>

// ============================================================
// Part 1 Required Helper Functions (game.cpp)
// ============================================================

/**
 * Gets a random location within the screen bounds.
 *
 * @return Point with x in [0, SCREEN_WIDTH) and y in [0, SCREEN_HEIGHT)
 */
Point getRandomLocation() {
    Point p;
    p.x = std::rand() % SCREEN_WIDTH;
    p.y = std::rand() % SCREEN_HEIGHT;
    return p;
}

/**
 * Gets a random velocity with x and y in [-1, 1].
 *
 * @return Point with velocity components in [-1.0, 1.0]
 */
Point getRandomVelocity() {
    int rx = std::rand() % 201;
    int ry = std::rand() % 201;

    Point v;
    v.x = (rx - 100) / 100.0;
    v.y = (ry - 100) / 100.0;
    return v;
}

// ============================================================
// Part 2 Required Collision Function (NOT a class member)
// ============================================================

/**
 * Determines whether two SpaceObjects intersect on screen.
 *
 * @param a Pointer to first SpaceObject
 * @param b Pointer to second SpaceObject
 * @return true if distance between centers <= sum of radii
 */
bool objectsIntersect(const SpaceObject* a, const SpaceObject* b) {
    Point pa = a -> getLocation();
    Point pb = b -> getLocation();

    double dx = pa.x - pb.x;
    double dy = pa.y - pb.y;

    double distSq = dx * dx + dy * dy;
    double radSum = a -> getRadius() + b -> getRadius();

    return distSq <= (radSum * radSum);
}

int main() {
    // ============================================================
    // Window generation
    // ============================================================
    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(static_cast<unsigned>(SCREEN_WIDTH),
                                   static_cast<unsigned>(SCREEN_HEIGHT))),
        "Asteroids - CS162"
    );
     
    window.setFramerateLimit(60);
    
    // Initial score, lives, game start/end status and invincibility status
    int score = 0;
    int lives = 3;
    bool gameOver = false;
    bool invincible = false;
    int invincibleTimer = 0;
    
    // Death Message
    sf::Font font;
    if (!font.openFromFile("PressStart2P-Regular.ttf")) {
        std::cout << "Font Failed to load!\n";
    }

    // ============================================================
    // Part 1: Seed RNG exactly as specified
    // ============================================================
    std::srand(42);

    // ============================================================
    // Part 0: Ship is a pointer + dynamically allocated
    // ============================================================
    SpaceObject* ship = new SpaceObject();


    ship -> setLocation(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0);
    ship -> setVelocity(0.0, 0.0);
    ship -> setRadius(20.0);
    ship -> setAngle(0.0);

    // ============================================================
    // Part 1: Array of asteroid pointers sized MAX_ASTEROIDS
    // - Initialize all entries to nullptr
    // ============================================================
    SpaceObject* asteroids[MAX_ASTEROIDS] = { nullptr };

    SpaceObject* photons[MAX_PHOTONS] = {nullptr};

    // Creates 5 asteroids in the first 5 slots

    for (int i = 0; i < 5; i++) {
        asteroids[i] = new SpaceObject();

        Point p = getRandomLocation();
        Point v = getRandomVelocity();

        asteroids[i] -> setLocation(p.x, p.y);
        asteroids[i] -> setVelocity(v.x, v.y);
        asteroids[i] -> setRadius(25.0);
        asteroids[i] -> setType(ASTEROID);
        asteroids[i] -> generateAsteroidShape();
    }

    // ============================================================
    // Game Loop
    // ============================================================
    while (window.isOpen()) {

        // ---------------------
        // 1) Handle events
        // ---------------------
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event -> is<sf::Event::Closed>()) {
                window.close();
            }
            if (event -> is <sf::Event::KeyPressed>()) {
                if (event -> getIf<sf::Event::KeyPressed>() -> code == sf::Keyboard::Key::Space && ship -> getType() == SHIP) {
                    for (int i = 0; i < MAX_PHOTONS; i++) {
                        if (photons[i] == nullptr) {

                            photons[i] = new SpaceObject();

                            photons[i] -> setType(PHOTON_TORPEDO);
                            photons[i] -> setRadius(2.0);

                            photons[i] -> setLocation(   
                                ship   -> getLocation().x,
                                ship   -> getLocation().y
                            );

                            photons[i] -> setVelocity(
                                ship   -> getVelocity().x, 
                                ship   -> getVelocity().y
                            );

                            photons[i] -> setAngle(ship -> getAngle());

                            photons[i] -> applyThrust(5.0);

                            break;
                        }
                    }
                }
            }

            // ===================================================================
            // Restart controls after user gets a game over
            // ===================================================================

            if (gameOver && event->is<sf::Event::KeyPressed>()) {
                auto key = event->getIf<sf::Event::KeyPressed>()->code;

                if (key == sf::Keyboard::Key::Y) {

                    // Reset game state
                    score = 0;
                    lives = 3;
                    gameOver = false;

                    // Delete all asteroids
                    for (int i = 0; i < MAX_ASTEROIDS; i++) {
                        delete asteroids[i];
                        asteroids[i] = nullptr;
                    }

                    // Recreate starting asteroids
                    for (int i = 0; i < 5; i++) {
                        asteroids[i] = new SpaceObject();
                        Point p = getRandomLocation();
                        Point v = getRandomVelocity();

                        asteroids[i]->setType(ASTEROID);
                        asteroids[i]->setLocation(p.x, p.y);
                        asteroids[i]->setVelocity(v.x, v.y);
                        asteroids[i]->setRadius(25.0);
                    }

                    // Reset ship
                    delete ship;
                    ship = new SpaceObject();
                    ship->setLocation(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0);
                    ship->setRadius(20.0);
                    ship->setAngle(0.0);

                }

                if (key == sf::Keyboard::Key::N) {
                    window.close();
                }
            }

        }

        // ---------------------
        // 2) Controls (ship only)
        // ---------------------
        if (!gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            ship->changeAngle(-ROT_SPEED);
        }
        if (!gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            ship->changeAngle(ROT_SPEED);
        }
        if (!gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            ship->applyThrust(0.7);
        }

        // ---------------------
        // 3) Update positions
        // ---------------------

        ship->updatePosition();

        // =================================
        // Respawn Logic
        // =================================
        if (ship -> getType() == SHIP_GONE && lives > 0) {

            lives--;

            delete ship;

            ship = new SpaceObject;

            ship -> setLocation(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0);
            ship -> setVelocity(0.0, 0.0);
            ship -> setRadius(20.0);
            ship -> setAngle(0.0);

            ship -> setInvincible(true);
        }

        for (int i = 0; i < MAX_ASTEROIDS; i++) {
            if (asteroids[i] != nullptr) {
                asteroids[i] -> updatePosition();
            }
        }
        for (int i = 0; i < MAX_PHOTONS; i++) {
            if (photons[i] != nullptr) {

                photons[i] -> updatePosition();

                if (photons[i] -> getTimesDrawn() > PHOTON_LIFESPAN) {
                    delete photons[i];
                    photons[i] = nullptr;
                }
            }
        }

        // -------------------------------------------------
        // 4) Part 2 + Part 3: Collision -> explode()
        // -------------------------------------------------
        for (int i = 0; i < MAX_ASTEROIDS; i++) {
            if (asteroids[i] != nullptr) {
                if (!ship -> isInvincible() && objectsIntersect(ship, asteroids[i]) 
                    && ship -> getType() == SHIP) {
                    ship -> explode();
                    
                    if (lives <= 1) {
                        gameOver = true;
                    }

                    break;
                }
            }
        }

        // Photon vs Asteroid collision
        for (int p = 0; p < MAX_PHOTONS; p++) {

            if (photons[p] == nullptr)
                continue;

            for (int a = 0; a < MAX_ASTEROIDS; a++) {

                if (asteroids[a] == nullptr)
                    continue;

                if (objectsIntersect(photons[p], asteroids[a])) {

                    double oldRadius = asteroids[a]->getRadius();
                    Point hitLocation = asteroids[a]->getLocation();

                    // Delete photon
                    delete photons[p];
                    photons[p] = nullptr;

                    // If asteroid large enough → split
                    if (oldRadius > 6.25) {

                        int created = 0;

                            for (int i = 0; i < MAX_ASTEROIDS && created < 2; i++) {

                                if (asteroids[i] == nullptr) {

                                    asteroids[i] = new SpaceObject();
                                    asteroids[i] -> setType(ASTEROID);
                                    asteroids[i] -> setRadius(oldRadius / 2.0);
                                    asteroids[i] -> generateAsteroidShape();
                                    asteroids[i] -> setLocation(hitLocation.x, hitLocation.y);

                                    Point v = getRandomVelocity();
                                    asteroids[i]->setVelocity(v.x * 2, v.y * 2);

                                    created++;
                                }
                            }
                    }

                    // Calculate Score

                    if (oldRadius == 25.0) {
                        score += 10;
                    } else if (oldRadius == 12.5) {
                        score += 20;
                    } else {
                        score += 40;
                    }

                    // Delete original asteroid
                    delete asteroids[a];
                    asteroids[a] = nullptr;

                    break;
                }
            }
        }


        // ---------------------
        // 5) Draw everything
        // ---------------------
        window.clear(sf::Color::Black);

        for (int i = 0; i < MAX_ASTEROIDS; i++) {
            if (asteroids[i] != nullptr) {
                asteroids[i] -> draw(window);
            }
        }

        for (int i = 0; i < MAX_PHOTONS; i++) {
            if (photons[i] != nullptr) {
                photons[i] -> draw(window);
            }
        }

        ship -> draw(window);

        // Score Display
        sf::Text scoreText(font);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(18);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(sf::Vector2f(10.f, 10.f));
        window.draw(scoreText);

        // ====================================================================
        // Death message displays after all lives lost
        // ====================================================================
        if (gameOver) {
            sf::Text gameOverText(font);
            gameOverText.setString("Game Over!");
            gameOverText.setCharacterSize(48);
            gameOverText.setFillColor(sf::Color::Red);

            // Get text bounds
            sf::FloatRect textBounds = gameOverText.getLocalBounds();

            // Set origin to center of death message
            gameOverText.setOrigin(
                textBounds.position + textBounds.size / 2.f
            );

            // Position in the middle of the window
            gameOverText.setPosition(sf::Vector2f(
                SCREEN_WIDTH / 2.0f,
                SCREEN_HEIGHT / 2.0f
            ));
            window.draw(gameOverText);

            // ============= Pulsing restart prompt =========================
            static float pulse = 0.f;
            pulse += 0.05f;

            int alpha = 128 + std::sin(pulse) * 127;

            sf::Text restartText(font);
            restartText.setString("Press Y to Restart\nPress N to Quit");
            restartText.setCharacterSize(18);
            restartText.setFillColor(sf::Color(255, 140, 0, alpha));

            sf::FloatRect rBounds = restartText.getLocalBounds();
            restartText.setOrigin(rBounds.position + rBounds.size / 2.f);
            restartText.setPosition({SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f + 40.f});

            window.draw(restartText);
        }

        window.display();
    }

    // ============================================================
    // Cleanup
    // ============================================================
    delete ship;
    ship = nullptr;

    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        delete asteroids[i];
        asteroids[i] = nullptr;
    }

    return 0;
}
