# DungineX

> Copyright &copy; Tony's Studio 2024

---

## Preface

To be continued...

---

## Quick Start

### Creating New Project

Open Visual Studio and create a new solution with C++ Empty Project. Then, clone this [repository](https://github.com/Lord-Turmoil/DungineX), copy DungineX directory into your solution and add DungineX as existing project. Next, make the following configuration for your project.

In "General", change "Output Directory" and "Intermediate Directory", `Bin\` is for output and `Build\` for intermediate.

```
$(SolutionDir)Bin\$(ProjectName)\$(Configuration)\
$(SolutionDir)Build\$(ProjectName)\$(Configuration)\
```

In "General", set C++ language standard to at least C++ 17.

In "C/C++ > General", add "Additional Include Directories".

```
$(SolutionDir)Include
$(SolutionDir)DungineX\Include
```

Then, in "C/C++ > Preprocessor" add "Preprocessor Definitions" for each profiles. `_RELEASE` for Release and `_PUBLISH` for Publish.

In "Linker > General", add "Additional Library Directories".

```
$(SolutionDir)Lib
```

For Debug profile, in "Linker > Input", add "Ignore Specific Default Libraries" to avoid some warnings.

```
msvcrt.lib
```

To avoid creating default console, go to "Linker > System" and change SubSystem to "Windows", then in "Linker > Advanced" set "Entry Point" to "mainCRTStartup" to use `main` instead of `WinMain`.

Now you're ready to build your amazing application with DungineX!🎉

---

## References

To implement the physics system of my game engine, I read the book *Game Physics Engine Development* by Ian Millington. For the framework and some implementation outside physics in game engine, I referenced [Hazel Engine](https://github.com/TheCherno/Hazel) by [Cherno](https://github.com/TheCherno).
