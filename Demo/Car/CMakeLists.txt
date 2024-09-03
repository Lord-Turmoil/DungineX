# ====================================================================
# Demonstration of physics simulation with particle system
# ====================================================================

project(Car)

add_executable(${PROJECT_NAME} Main.cpp Car.cpp CarApp.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ${DGEX_LIB})
