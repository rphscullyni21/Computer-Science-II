/* 
*   File: constants.h
*   Name: Nick Cully
*   Version: Part 4 
*   Date: 02/11/2026
*   Class: CS - 162 - (02)
*   Description: Assignment 3.2, Final part of the Asteroids clone. Creates an array of finite torpedos with pointers, torpedos have a lifetime, torpedos delete after collisions, and make the asteroids split for full credit.
*
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const double ROT_SPEED = 3.0;
const double THRUST = 0.1;
const double DRAG = 0.99;
const int PHOTON_LIFESPAN = 100;

const int MAX_ASTEROIDS = 20;
const int MAX_PHOTONS = 10;

#endif
