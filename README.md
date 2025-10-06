# DungineX

> [!WARNING]
> This project is still in early development.

Copyright &copy; New Desire Studios 2025

---

|  [main](https://github.com/Lord-Turmoil/DungineX/tree/main)  |   [dev](https://github.com/Lord-Turmoil/DungineX/tree/dev)   |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| [![Build (main)](https://github.com/Lord-Turmoil/DungineX/actions/workflows/build.yml/badge.svg?branch=main)](https://github.com/Lord-Turmoil/DungineX/actions/workflows/build.yml) | [![Build (dev)](https://github.com/Lord-Turmoil/DungineX/actions/workflows/build.yml/badge.svg?branch=dev)](https://github.com/Lord-Turmoil/DungineX/actions/workflows/build.yml) |

## Overview

DungineX is a 2D game engine built on top of [SDL 3](https://github.com/libsdl-org/SDL), supporting both Windows and Linux.

> [!WARNING]
> Although DungineX targets both Windows and Linux, it prioritizes Windows, and some features may not be available on Linux.

## Project Setup

DungineX uses git submodules to manage dependencies. After you clone the repository, init and update all submodules.

```bash
git submodule update --init --recursive
```

Files used for unit testing will be automatically copied to their locations during CMake configuration.

> [!WARNING]
>
> SDL required a bunch of dependencies on Linux. For more information, see [Build Dependencies](https://github.com/libsdl-org/SDL/blob/main/docs/README-linux.md#build-dependencies).

## Build

It is recommended to use [Ninja](https://ninja-build.org/) as the generator.

> [!NOTE]
>
> On Windows, you can simply use Visual Studio with bundled Ninja. On Linux, you can install Ninja using `apt install ninja-build`.

```bash
cmake -S . -B build -G "Ninja"
cmake --build build --parallel
```

You can also use presets defined in `CMakePresets.json`.

```bash
cmake --preset list # list avalable presets
cmake --preset linux-debug-static
```

## Use DungineX

> [!NOTE]
>
> There is some examples under `Demo/`.

You can set up DungineX as a submodule in your own project:

```bash
git submodule add https://github.com/Lord-Turmoil/DungineX.git
```

Then, in your `CMakeLists.txt`, you can include DungineX like this:

```cmake
add_subdirectory(DungineX)
target_link_libraries(YourProject PRIVATE DgeX::DgeX)
```

If you want to use static linking, you can set `DGEX_USE_SHARED` to `OFF`.
