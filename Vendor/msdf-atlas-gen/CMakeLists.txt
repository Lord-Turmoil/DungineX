project(msdf-atlas-gen)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/msdf-atlas-gen/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/msdf-atlas-gen/*.hpp
)

add_library(msdf-atlas-gen ${SOURCE_FILES})

target_include_directories(msdf-atlas-gen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(msdf-atlas-gen PUBLIC msdfgen)
