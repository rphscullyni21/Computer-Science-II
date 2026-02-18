/* 
*   File: spaceObject.h
*   Name: Nick Cully
*   Version: Part 4 
*   Date: 02/11/2026
*   Class: CS - 162 - (02)
*   Description: Assignment 3.2, Final part of the Asteroids clone. Creates an array of finite torpedos with pointers, torpedos have a lifetime, torpedos delete after collisions, and make the asteroids split for full credit.
*
*/

#ifndef SPACEOBJECT_H
#define SPACEOBJECT_H
#include "constants.h"
#include <SFML/Graphics.hpp>
#include <vector>

// ===================== Constants ==========================================================

struct Point {
    double x;
    double y;
};

enum SpaceObjType {
    SHIP,
    ASTEROID,
    PHOTON_TORPEDO,
    SHIP_EXPLODING,
    SHIP_GONE
};

class SpaceObject {
    private:

    SpaceObjType type;
    Point location;
    Point velocity;
    double radius;
    double angle;
    int timesDrawn;
    bool invincible;
    int invincibleTimer;

    std::vector<sf::Vector2f> vertices;

    public:

    SpaceObject();
    
    /*
    *   Creates the SpaceObject with the following initial values:
    *
    *   @param type Object type (SHIP, ASTEROID, PHOTON_TORPEDO)
    *   @param location Initial position of the object
    *   @param velocity Initial velocity of the object
    *   @param radius Collision radius of the object
    *   @param angle Initial orientation angle (degrees)
    */
    
    SpaceObject(SpaceObjType type, Point location, Point velocity, double radius, double angle);
    
    // ===================== Mutators(Setters) ===========================================
    /**
     * Sets the radius of the object if given radius is actually valid.
     *
     * @param r New radius value
     * @return true if the radius was set, otherwise false
    */
    bool setRadius(double r);

    /**
     * Sets the location of the object, and wraps the bounds if necessary.
     *
     * @param x New x-coordinate
     * @param y New y-coordinate
    */
    void setLocation(double x, double y);

    /**
     * Sets the velocity of the object.
     *
     * @param vx x-direction Velocity
     * @param vy y-direction Velocity
    */
    void setVelocity(double vx, double vy);

    /**
     * Sets the angle of attack of the object.
     *
     * @param a New angle in degrees
    */
    void setAngle(double a);

    /**
     * Adjusts the angle the object is pointing in by a given amount.
     *
     * @param delta Amount to change the angle by (in degrees)
    */
    void changeAngle(double delta);

    // ===================== Accessors(Getters) ===========================================

    /**
     * Gets the object type.
     *
     * @return SpaceObjType of the object
    */
    SpaceObjType getType() const;

    /**
     * Gets the current location of the object.
     *
     * @return Point representing the location
    */
    Point getLocation() const;

    /**
     * Gets the current velocity of the object.
     *
     * @return Point representing the velocity
    */
    Point getVelocity() const;

    /**
     * Gets the collision radius of the object.
     *
     * @return Radius value
    */
    double getRadius() const;

    /**
     * Gets the current orientation angle.
     *
     * @return Angle in degrees
    */
    double getAngle() const;

    //======================= Behavior Functions ============================================

    /**
     * Updates the object's position depending on its given velocity.
    */
    void updatePosition();

    /**
     * Applies forward thrust to the object.
     * Only has an effect when the object is a functioning ship.
    */
    void applyThrust(float thrustAmount);

    /**
     * Draws the exploding ship animation.
     *
     * @param window SFML render window
     */
    void drawExplodingShip(sf::RenderWindow& window);

    /**
     * Causes the ship to explode.
     * Changes state to SHIP_EXPLODING and stops movement.
    */
    void explode();

    /**
     * Draws the object based on its current state.
     *
     * @param window SFML render window to draw to
    */
    void draw(sf::RenderWindow& window);

    void setType(SpaceObjType t);

    /**
     * Will return the current state of the object (for testing/debugging).
    */
    void dumpData() const;

    int getTimesDrawn() const;

    void setInvincible(bool value);

    bool isInvincible() const;

    void generateAsteroidShape();

};
#endif
