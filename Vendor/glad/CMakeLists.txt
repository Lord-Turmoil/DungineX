project(glad)

add_library(glad STATIC
    glad/glad.h
    glad/glad.c
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
