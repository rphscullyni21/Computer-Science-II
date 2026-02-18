/* 
*   File: spaceObject.cpp
*   Name: Nick Cully
*   Version: Part 4 
*   Date: 02/11/2026
*   Class: CS - 162 - (02)
*   Description: Assignment 3.2, Final part of the Asteroids clone. Creates an array of finite torpedos with pointers, torpedos have a lifetime, torpedos delete after collisions, and make the asteroids split for full credit.
*/

#include "spaceObject.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>

using namespace std;

/**
 * Default constructor
 */
SpaceObject::SpaceObject() {
    type = SHIP;
    timesDrawn = 0;
    invincible = false;
    invincibleTimer = 0;

    location = {0.0, 0.0};
    velocity = {0.0, 0.0};

    radius = 10.0;
    angle = 0.0;
}

/**
 * Parameterized constructor
 */
SpaceObject::SpaceObject(SpaceObjType type, Point location, Point velocity,
                         double radius, double angle) {
    this->type = type;
    this->location = location;
    this->velocity = velocity;
    this->angle = angle;

    timesDrawn = 0;
    invincible = false;
    invincibleTimer = 0;

    setRadius(radius);
}

void SpaceObject::generateAsteroidShape() {

    vertices.clear();

    const int POINT_COUNT = 12;

    for (int i = 0; i < POINT_COUNT; i++) {

        float angleDeg = (360.f / POINT_COUNT) * i;
        float angleRad = angleDeg * 3.14159265f / 180.f;

        float variation = 0.8f + static_cast<float>(std::rand()) / RAND_MAX * 0.4f;
        float r = static_cast<float>(radius) * variation;

        float x = std::cos(angleRad) * r;
        float y = std::sin(angleRad) * r;

        vertices.push_back(sf::Vector2f(x, y));
    }
}

/**
 * Set radius if valid
 */
bool SpaceObject::setRadius(double r) {
    if (r > 0) {
        radius = r;
        return true;
    }
    return false;
}

/**
 * Set velocity
 */
void SpaceObject::setVelocity(double vx, double vy) {
    velocity.x = vx;
    velocity.y = vy;
}

/**
 * Set angle
 */
void SpaceObject::setAngle(double a) {
    angle = a;
    while (angle >= 360.0) angle -= 360.0;
    while (angle < 0.0) angle += 360.0;
}

/**
 * Change angle (SHIP only)
 */
void SpaceObject::changeAngle(double delta) {
    if (type != SHIP)
        return;

    angle += delta;
    if (angle >= 360.0) angle -= 360.0;
    if (angle < 0.0) angle += 360.0;
}

/**
 * Apply thrust (SHIP only)
 */
void SpaceObject::applyThrust(float thrustAmount) {
    if (type != SHIP && type != PHOTON_TORPEDO)
        return;

    double angleRad = angle * 3.14159265 / 180.0;
    velocity.x += std::cos(angleRad) * thrustAmount;
    velocity.y += std::sin(angleRad) * thrustAmount;

    double speed = std::sqrt(velocity.x * velocity.x +
                         velocity.y * velocity.y);

    // Cap the speed to avoid going mach jesus off the screen into an asteroid.
    double MAX_SPEED = 3.0;

    if (speed > MAX_SPEED) {
        velocity.x = (velocity.x / speed) * MAX_SPEED;
        velocity.y = (velocity.y / speed) * MAX_SPEED;
    }

}

/**
 * Cause ship to explode
 */
void SpaceObject::explode() {
    if (type != SHIP)
        return;

    type = SHIP_EXPLODING;
    velocity = {0.0, 0.0};
}

/**
 * Update position or explosion
 */
void SpaceObject::updatePosition() {
    if (invincible) {
        invincibleTimer--;
        if (invincibleTimer <= 0) {
            invincible = false;
        }
    }

    if (type == SHIP_GONE)
        return;

    if (type == SHIP_EXPLODING) {
        radius += 0.8;
        if (radius >= 40.0)
            type = SHIP_GONE;
        return;
    }

    setLocation(location.x + velocity.x,
                location.y + velocity.y);
}

/**
 * Screen wrap location
 */
void SpaceObject::setLocation(double x, double y) {
    location.x = x;
    location.y = y;

    if (location.x < 0) location.x += SCREEN_WIDTH;
    if (location.x >= SCREEN_WIDTH) location.x -= SCREEN_WIDTH;
    if (location.y < 0) location.y += SCREEN_HEIGHT;
    if (location.y >= SCREEN_HEIGHT) location.y -= SCREEN_HEIGHT;
}

/**
 * Getters
 */
SpaceObjType SpaceObject::getType() const { return type; }
Point SpaceObject::getLocation() const { return location; }
Point SpaceObject::getVelocity() const { return velocity; }
double SpaceObject::getRadius() const { return radius; }
double SpaceObject::getAngle() const { return angle; }

/**
 * Draw object
 */
void SpaceObject::draw(sf::RenderWindow& window) {
    timesDrawn++;

    // Dont redraw the ship if its gone
    if (type == SHIP_GONE) return;

    // ================== SHIP ============================

    if (type == SHIP) {
        sf::CircleShape shipShape(static_cast<float>(radius));
        // Pulse alpha if invincible
        if (invincible) {
            // invincibleTimer counts down from 180 -> 0
            float t = static_cast<float>(invincibleTimer) * 0.25f;
            int alpha = static_cast<int>(128 + std::sin(t) * 127);
            if (alpha < 30) alpha = 30;   // keep it visible
            if (alpha > 255) alpha = 255;

            shipShape.setFillColor(sf::Color(255, 255, 255, static_cast<std::uint8_t>(alpha)));
        } else {
            shipShape.setFillColor(sf::Color::White);
        }

        shipShape.setOrigin(sf::Vector2f(
            static_cast<float>(radius),
            static_cast<float>(radius)
        ));
        shipShape.setPosition(sf::Vector2f(
            static_cast<float>(location.x),
            static_cast<float>(location.y)
        ));
        shipShape.setRotation(sf::degrees(static_cast<float>(angle)));

        sf::RectangleShape nose(sf::Vector2f(static_cast<float>(radius), 2.f));

        nose.setFillColor(sf::Color::Red);
        nose.setOrigin(sf::Vector2f(0.f, 1.f));
        nose.setPosition(shipShape.getPosition());
        nose.setRotation(shipShape.getRotation());

        window.draw(shipShape);
        window.draw(nose);
    } 

    // ===================== ASTEROID =========================

    if (type == ASTEROID) {

        sf::ConvexShape rock;
        rock.setPointCount(vertices.size());

        for (size_t i = 0; i < vertices.size(); i++) {
            rock.setPoint(i, vertices[i]);
        }

        rock.setFillColor(sf::Color(120, 120, 120));
        rock.setPosition(sf::Vector2f(
                        static_cast<float>(location.x),
                        static_cast<float>(location.y)
                    ));

        window.draw(rock);
        return;
    }


    // ================= PHOTON TORPEDO ============================
    if (type == PHOTON_TORPEDO) {
        sf::CircleShape p(static_cast<float>(radius));
        p.setFillColor(sf::Color::Yellow); // easy to see
        p.setOrigin(sf::Vector2f(static_cast<float>(radius), static_cast<float>(radius)));
        p.setPosition(sf::Vector2f(
            static_cast<float>(location.x),
            static_cast<float>(location.y)
        ));
        window.draw(p);
        return;
    }
    
    // =================== EXPLOSION ===============================
    if (type == SHIP_EXPLODING) {
        drawExplodingShip(window);
        return;
    } 
}

void SpaceObject::setType(SpaceObjType t) {
    type = t;
}

/**
 * Draw explosion animation
 */
void SpaceObject::drawExplodingShip(sf::RenderWindow& window) {
    sf::CircleShape explosion(radius);
    explosion.setFillColor(sf::Color::Red);
    explosion.setOrigin(sf::Vector2f(
        static_cast<float>(radius),
        static_cast<float>(radius)
    ));
    explosion.setPosition(sf::Vector2f(
        static_cast<float>(location.x),
        static_cast<float>(location.y)
    ));

    window.draw(explosion);
}

int SpaceObject::getTimesDrawn() const {
    return timesDrawn;
}

void SpaceObject::setInvincible(bool value) {
    invincible = value;
    if (value) {
        invincibleTimer = 180;
    }
}

bool SpaceObject::isInvincible() const {
    return invincible;
}

/**
 * Debug dump
 */
void SpaceObject::dumpData() const {
    cout << "Type: " << type << endl;
    cout << "Location: (" << location.x << ", " << location.y << ")" << endl;
    cout << "Velocity: (" << velocity.x << ", " << velocity.y << ")" << endl;
    cout << "Radius: " << radius << endl;
    cout << "Angle: " << angle << endl;
}
