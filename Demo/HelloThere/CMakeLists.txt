# ====================================================================
# HelloThere Demo
# --------------------------------------------------------------------
# This is the simplest way to use DungineX in your projects.
# ====================================================================

cmake_minimum_required(VERSION 3.21)

project(Hello-There)

if(DGEX_CONSOLE_APP)
    add_executable(HelloThere Main.cpp)
else()
    add_executable(HelloThere WIN32 Main.cpp)
endif()

target_link_libraries(HelloThere PRIVATE DgeX::Lib)
