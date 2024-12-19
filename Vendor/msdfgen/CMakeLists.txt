project(msdfgen)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/msdfgen/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/msdfgen/*.hpp
)

add_library(msdfgen ${SOURCE_FILES})

target_include_directories(msdfgen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(msdfgen PUBLIC freetype tinyxml2 lodepng)
