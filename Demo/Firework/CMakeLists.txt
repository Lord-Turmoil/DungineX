# ====================================================================
# Demonstration of basic particle system
# ====================================================================

project(Firework)

add_executable(${PROJECT_NAME} Main.cpp Firework.cpp FireworkApp.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ${DGEX_LIB})
