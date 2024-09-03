# ====================================================================
# Demonstration of basic application
# ====================================================================

project(AppDemo)

add_executable(${PROJECT_NAME} Main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ${DGEX_LIB})
