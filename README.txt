# 🚀 Asteroids – Dynamic Memory Allocation and Classes  
**CS162 – Assignment 3.2 (Extra Features Version)**  
Author: Nick Cully  

---

## 📌 Overview

This project is a recreation of the classic **Asteroids** arcade game, developed in C++ using the **SFML graphics library**.

The base assignment focused on:
- Object-oriented programming
- Dynamic memory allocation using pointers
- Collision detection
- Splitting asteroids
- Managing arrays of dynamically allocated objects

After completing all required features, additional enhancements were added to create a more polished and arcade-like experience.

---

## 🎮 Controls

| Key | Action |
|-----|--------|
| ←   | Rotate Left |
| →   | Rotate Right |
| ↑   | Thrust |
| Space | Fire Photon Torpedo |

---

## ✅ Base Assignment Features

- Ship dynamically allocated using `new`
- Asteroids stored as an array of pointers (`SpaceObject* asteroids[]`)
- Photon torpedoes stored as an array of pointers
- Collision detection via `objectsIntersect()`
- Asteroid splitting into two smaller asteroids
- Torpedo lifespan management (`PHOTON_LIFESPAN`)
- Proper deletion of objects to prevent memory leaks
- Screen wrapping behavior
- Destructor-style cleanup before program exit

---

## ⭐ Extra Features Added

### 🟡 1. Score Counter
- Points awarded for destroying asteroids:
  - Large asteroid → +10
  - Medium asteroid → +20
  - Small asteroid → +40
- Score displayed in the top-left corner.

---

### 🔥 2. Explosion Animation
- When the ship collides with an asteroid:
  - Ship enters `SHIP_EXPLODING` state
  - Explosion animation grows outward
  - Ship eventually transitions to `SHIP_GONE`

---

### ☠️ 3. Game Over Screen
- Displays **"Game Over!"** centered on screen.
- Prevents further ship movement or interaction.

---

### 🔁 4. Restart Prompt
- Pulsing message:
- `Y` resets:
- Score
- Asteroids
- Ship
- `N` closes the game window.

---

### 🧠 5. Clean Memory Management

All dynamically allocated objects are:
- Deleted when destroyed
- Set to `nullptr` after deletion
- Properly cleaned up at program termination

No memory leaks.

---

## 🏗 Technical Implementation Details

- Language: **C++**
- Graphics Library: **SFML 3.0**
- OOP Design:
- `SpaceObject` class handles all object behavior
- Encapsulation enforced through private data members
- Collision detection using radius-based distance formula
- Screen wrapping via coordinate adjustment
- Uses `std::optional<sf::Event>` for event handling

---

## 📂 Project Structure
/Asteroids
constants.h
spaceObject.h
spaceObject.cpp
game.cpp
PressStart2P-Regular.ttf
---

## 🧩 Concepts Demonstrated

- Dynamic memory allocation
- Pointer arrays
- Object-oriented design
- Encapsulation
- Enum states for object behavior
- File organization across multiple compilation units
- Real-time game loop architecture

---

## 💭 Reflection

This project began as a structured dynamic memory assignment and evolved into a fully playable arcade-style game. After building the required functionality, additional features were added to improve usability and presentation while maintaining proper memory management and class structure.

It demonstrates not only understanding of pointers and object-oriented programming, but also growth in structuring larger multi-file C++ programs.

---

## 🛠 Build Instructions

Compile using:

g++ game.cpp spaceObject.cpp -I "SFML/include" -L "SFML/lib"
-lsfml-graphics -lsfml-window -lsfml-system -o asteroids


Ensure required SFML `.dll` files are included when running on Windows.

---

## 🎯 Final Notes

This repository contains the **enhanced version** of the assignment.  
A separate zip file was submitted containing only the base required features for grading.


