# ====================================================================
# Top-level CMakeLists.txt file for the DungineX project
# ====================================================================

cmake_minimum_required(VERSION 3.21)

include(cmake/utils.cmake)

# --------------------------------------------------------------------
# Project Options
# --------------------------------------------------------------------

project(DungineX-Engine)

if(NOT WIN32)
    message(FATAL_ERROR "DungineX targets only Windows!")
endif()

# Set default build type if not specified.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "CMake build type" FORCE)
endif()

# Check if we are building the master project or a subproject.
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(DGEX_MASTER_PROJECT ON)
else()
    set(DGEX_MASTER_PROJECT OFF)
endif()

# By default, DungineX is shipped as a shared library with SDL included,
# so that the distribution won't carry a bunch of SDL .dll files.
option(DGEX_USE_SHARED "Use shared libraries for DungineX" ON)
option(DGEX_USE_SHARED_SDL "Use shared libraries for SDL" OFF)

if(DGEX_USE_SHARED)
    option(DGEX_BUILD_SHARED "Build DungineX as shared libraries (.dll)" ON)
    option(DGEX_BUILD_STATIC "Build DungineX as static libraries (.lib)" OFF)
else()
    option(DGEX_BUILD_SHARED "Build DungineX as shared libraries (.dll)" OFF)
    option(DGEX_BUILD_STATIC "Build DungineX as static libraries (.lib)" ON)
endif()

if(DGEX_MASTER_PROJECT)
    option(DGEX_BUILD_DEMO "Build demo projects" ON)
    option(DGEX_BUILD_TEST "Build unit tests" ON)

    option(DGEX_PUBLISH "Build DungineX for publishing" OFF)
else()
    option(DGEX_BUILD_DEMO "Build demo projects" OFF)
    option(DGEX_BUILD_TEST "Build unit tests" OFF)

    option(DGEX_PUBLISH "Build DungineX for publishing" ON)
endif()

# If DungineX is the master project, then change the output directories for a more
# friendly organization of the build artifacts.
if(DGEX_MASTER_PROJECT)
    # FIXME:
    # If using Visual Studio as the generator, the output directories will have an
    # additional configuration subdirectory (e.g., Debug, Release, etc.).
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output/lib")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output/bin")
endif()

# --------------------------------------------------------------------
# Compiler Settings & Options
# --------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    if(NOT CMAKE_GENERATOR STREQUAL "Ninja")
        # Parallelize each target, unless Ninja is the generator.
        add_definitions(/MP)				
    endif()
endif()

if(DGEX_MASTER_PROJECT)
    option(DGEX_STRICT_WARNINGS "Enable strict warnings" ON)
else()
    option(DGEX_STRICT_WARNINGS "Enable strict warnings" OFF)
endif()

if(DGEX_PUBLISH)
    option(DGEX_CONSOLE_APP "Build console application with DungineX" OFF)
else()
    option(DGEX_CONSOLE_APP "Build console application with DungineX" ON)
endif()

option(DGEX_ENABLE_ASSERT "Enable assertions in DungineX" ON)

# --------------------------------------------------------------------
# Targets
# --------------------------------------------------------------------

# Adding vendor libraries.
add_subdirectory(Vendor)

# Adding the main DungineX library.
if(DGEX_BUILD_TEST AND NOT DGEX_BUILD_STATIC)
    # Tests require the static library.
    set(DGEX_BUILD_STATIC ON)
endif()
add_subdirectory(DungineX)

# Adding demo projects.
if(DGEX_BUILD_DEMO)
    add_subdirectory(Demo)
endif()

# Adding unit tests.
if(DGEX_BUILD_TEST)
    if(DGEX_MASTER_PROJECT)
        # Reset the output directory to the binary directory for unit tests.
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "")
    endif()
    
    enable_testing()
    add_subdirectory(Tests)
endif()

# --------------------------------------------------------------------
# Additional Configurations
# --------------------------------------------------------------------

# A more convenient target for the DungineX library.
dgex_get_target_name(DgeX::Lib dgex_target)
add_library(DgeX::DgeX ALIAS ${dgex_target})
