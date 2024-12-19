project(stb)

add_library(${PROJECT_NAME} STATIC
    stb/stb_image.cpp
    stb/stb_image.h
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
