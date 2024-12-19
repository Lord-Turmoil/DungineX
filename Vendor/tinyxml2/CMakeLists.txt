project(tinyxml2 VERSION 10.0.0)

add_library(${PROJECT_NAME} STATIC
    tinyxml2.cpp
    tinyxml2.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
