# DungineX

> Copyright &copy; Tony's Studio 2024 - 2025

---

> [!WARNING]
>
> This project is still under construction.

[![Build-and-Test](https://github.com/Lord-Turmoil/DungineX/actions/workflows/build-and-test.yml/badge.svg?branch=main)](https://github.com/Lord-Turmoil/DungineX/actions/workflows/build-and-test.yml)

## Preface

I've always have a passion for game development, especially the engine behind it. So years ago, I created [Dungeon](https://github.com/Lord-Turmoil/Dungeon) with  my own 2D game engine Dungine (Dungeon Engine) form scratch. It has an entity component system and works well with RPG game. Besides, it is bundled with a fancy GUI library. All of these are implemented by myself, but it is a pity that I didn't realize a robust physics system. So Dungine can only handle basic rectangle collision.

Recently, I happened to find the book *Game Physics Engine Development* and decided to improve my engine and learn about the "real" physics. So here I am, making **DungineX**, the next generation of Dungine with physics support. By the way, X means physics.

---

## Examples

> Currently documentation is not available, so the only thing you can refer to is the demos in this repo.

This repository contains the complete source code of **DungineX** and some demos. The demos are placed under directory "Demo". Currently, there are four demos.

### AppDemo

Basic UI and responsive layout.

### Firework

Particle system with no collision. By pressing number 0 ~ 9, you can get different fireworks shooting from the cursor. Press ESC to exit.

### Platform

Particle system with constraints, and the use of `ParticleWorld`.

Use 'A' and 'D' to move the payload, and 'S' to reset it. Use 'Q', 'Z' and 'E', 'C' to make the bungee on two sides longer or shorter. Press ESC to exit.

### Car

This is a comprehensive demo for the particle engine. It simply uses particles to simulate rigid body, and handles circle - line collision.

You can control a car with 'W', 'S' for gas and 'A', 'D' for balance. Press 'R' to reset the car if you fall over, and press 'F' to reset the map and start again. Press ESC to exit.

---

## Quick Start

This is a standard CMake project, so you can simply add it by the following commands.

```cmake
add_subdirectory(DungineX)
target_link_libraries(${YOUR_PROJECT_NAME} PRIVATE ${DGEX_LIB})
```

In your source files, add `#include <DgeX/DgeX.h>` to get full features of DungineX!😃

### Set up application

*To be continued...*

---

## References

To implement the physics system of my game engine, I read the book *Game Physics Engine Development* by Ian Millington. For the framework and some implementation outside physics in game engine, I referenced [Hazel Engine](https://github.com/TheCherno/Hazel) by [Cherno](https://github.com/TheCherno).

---

## To Do

- [x] Application framework
- [ ] Physics engine
  - [x] Particle system
  - [ ] Rigid body system
- [ ] Integrated GUI
