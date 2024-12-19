# ====================================================================
# Top-level CMakeLists.txt file for the DungineX project
# ====================================================================

cmake_minimum_required(VERSION 3.21)

include(cmake/utils.cmake)

# --------------------------------------------------------------------
# Project configuration
# --------------------------------------------------------------------
dgex_extract_version()

project(DungineX
    VERSION ${DGEX_VERSION}
    DESCRIPTION "A 2D game engine with physics and GUI support"
    LANGUAGES CXX)
message(STATUS "Building DungineX ${DGEX_VERSION}")

option(DGEX_OPENGL "Use OpenGL" ON)
option(DGEX_BUILD_DEMO "Build demo projects" ON)
option(DGEX_ENABLE_ASSERTION "Enable assertions" ON)

# --------------------------------------------------------------------
# Project options
# --------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

if(NOT DEFINED DGEX_MASTER_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(DGEX_MASTER_PROJECT ON)
        message(STATUS "DungineX is the master project")
    else()
        set(DGEX_MASTER_PROJECT OFF)
        message(STATUS "DungineX is a sub-project")
    endif()
endif()

# --------------------------------------------------------------------
# Compiler settings
# --------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)

# --------------------------------------------------------------------
# Targets
# --------------------------------------------------------------------
add_subdirectory(Vendor)
message(STATUS "DungineX 3rd-party dependencies: ${DGEX_VENDORS}")
foreach(vendor ${DGEX_VENDORS})
    dgex_disable_crt_warnings(${vendor})
    dgex_disable_warnings(${vendor})
endforeach()

add_subdirectory(DungineX)
if(DGEX_MASTER_PROJECT)
    dgex_enable_warnings(${DGEX_LIB})
endif()
target_link_libraries(${DGEX_LIB} ${DGEX_VENDORS})

if(DGEX_BUILD_DEMO)
    message(STATUS "Building DungineX demo projects")
    add_subdirectory(Demo)
endif()

