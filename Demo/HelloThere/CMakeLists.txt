# ====================================================================
# HelloThere Demo
# --------------------------------------------------------------------
# This is the simplest way to use DungineX in your projects.
# ====================================================================

cmake_minimum_required(VERSION 3.21)

project(Hello-There)

add_executable(HelloThere Main.cpp)
target_link_libraries(HelloThere PRIVATE DgeX::Lib)
