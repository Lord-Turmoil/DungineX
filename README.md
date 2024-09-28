# DungineX

> Copyright &copy; Tony's Studio 2024

---

> **WARNING: This project is still under construction.**

## Preface

I've always have a passion for game development, especially the engine behind it. So years ago, I created [Dungeon](https://github.com/Lord-Turmoil/Dungeon) with  my own 2D game engine Dungine (Dungeon Engine) form scratch. It has an entity component system and works well with RPG game. Besides, it is bundled with a fancy GUI library. All of these are implemented by myself, but it is a pity that I didn't realize a robust physics system. So Dungine can only handle basic rectangle collision.

Recently, I happened to find the book *Game Physics Engine Development* and decided to improve my engine and learn about the "real" physics. So here I am, making **DungineX**, the next generation of Dungine with physics support. By the way, X means physics.

---

## Examples

> Currently documentation is not available, so the only thing you can refer to is the demos in this repo.

This repository contains the complete source code of **DungineX** and some demos. The demos are placed under directory "Demo". Currently, there are three demos.

After building the solution, the demo's executables will be output to "Bin/Demo/{ProjectName}/" directory.

### Firework

This demo shows the basic use of particles. By pressing number 0 ~ 9, you can get different fireworks shooting from the cursor. Press ESC to exit.

### Platform

This demo shows the basic use of particles and constraints. Unlike Firework, it uses the particle world to manage all particles.

Use 'A' and 'D' to move the payload, and 'S' to reset it. Use 'Q', 'Z' and 'E', 'C' to make the bungee on two sides longer or shorter. Press ESC to exit.

### Car

This is a comprehensive demo for the particle engine. It simply uses particles to simulate rigid body, and handles circle - line collision. Also, it integrates particles for the dust left behind.

You can control a car with 'W', 'S' for gas and 'A', 'D' for balance. Press 'R' to reset the car if you fall over, and press 'F' to reset the map and start again. Press ESC to exit.

---

## Quick Start

### Creating new project

Open Visual Studio and create a new solution with C++ Empty Project. Then, clone this [repository](https://github.com/Lord-Turmoil/DungineX), copy DungineX directory into your solution and add DungineX as existing project. Next, make the following configuration for your project.

If you care about the output location, then in "General", change "Output Directory" and "Intermediate Directory", `Bin\` is for output and `Build\` for intermediate. This is default for DungineX project.

```
$(SolutionDir)Bin\$(ProjectName)\$(Configuration)\
$(SolutionDir)Build\$(ProjectName)\$(Configuration)\
```

In "General", set C++ language standard to at least C++ 17.

In "C/C++ > General", add "Additional Include Directories".

```
$(SolutionDir)DungineX\Include
$(SolutionDir)DungineX\Vendor\Include
```

In "Linker > General", add "Additional Library Directories".

```
$(SolutionDir)Lib
```

For Debug profile (only), in "Linker > Input", add "Ignore Specific Default Libraries" to avoid some warnings.

```
msvcrt.lib
libcmt.lib
```

To avoid creating default console, go to "Linker > System" and change SubSystem to "Windows", then in "Linker > Advanced" set "Entry Point" to "mainCRTStartup" to use `main` instead of `WinMain`.

Now you're ready to build your amazing application with DungineX!🎉

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
