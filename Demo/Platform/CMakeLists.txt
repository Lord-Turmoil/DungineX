# ====================================================================
# Demonstration of basic particle system with constraints
# ====================================================================

project(Platform)

add_executable(${PROJECT_NAME} Main.cpp PlatformApp.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ${DGEX_LIB})
