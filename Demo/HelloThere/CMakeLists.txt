# ====================================================================
# HelloThere Demo
# --------------------------------------------------------------------
# This is the simplest way to use DungineX in your projects.
# ====================================================================

cmake_minimum_required(VERSION 3.21)

project(Hello-There)

if(DGEX_CONSOLE_APP)
    add_executable(HelloThere Main.cpp)
else()
    add_executable(HelloThere WIN32 Main.cpp)
endif()

target_link_libraries(HelloThere PRIVATE DgeX::DgeX)

macro(copy_asset filename)
    set(outname "${CMAKE_BINARY_DIR}/output/bin/${filename}")
    add_custom_command(POST_BUILD
        TARGET HelloThere
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_LIST_DIR}/${filename}" "${outname}"
		DEPENDS "${filename}"
	)
endmacro()

copy_asset("gs_tiger.svg")