# DungineX

> [!WARNING]
> This project is still in early development.

Copyright &copy; New Desire Studios 2025

---

[![Windows Build](https://github.com/Lord-Turmoil/DungineX/actions/workflows/windows.yml/badge.svg?branch=main)](https://github.com/Lord-Turmoil/DungineX/actions/workflows/windows.yml)

## Overview

DungineX is a 2D game engine built on top of [SDL 3](https://github.com/libsdl-org/SDL).

## Project Setup

DungineX is built using [CMake](https://cmake.org/). First, you need to fetch all submodules:

```bash
git submodule update --init --recursive
```

For unit testing, some files should be copied to the build directory. Rerun CMake configuration to automatically copy them. 


## Use DungineX

You can set up DungineX as a submodule in your own project:

```bash
git submodule add https://github.com/Lord-Turmoil/DungineX.git
```

Then, in your `CMakeLists.txt`, you can include DungineX like this:

```cmake
add_subdirectory(DungineX)
target_link_libraries(YourProject PRIVATE DgeX::DgeX)
```

If you want to use static linking, you can pass `DGEX_USE_SHARED=OFF`.
