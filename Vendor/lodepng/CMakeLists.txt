project(lodepng)

add_library(${PROJECT_NAME} STATIC
    lodepng.cpp
    lodepng.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
