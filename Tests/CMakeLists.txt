# ====================================================================
# DungineX Unit Tests
# ====================================================================

message(STATUS "Build DungineX unit tests")

set(tests 
    Style
    Version
    Expected
    Strings
    XmlDocument
)

foreach(test ${tests})
    file(GLOB_RECURSE found_file RELATIVE ${CMAKE_CURRENT_LIST_DIR} "${test}Test.cpp")
    if(found_file)
        add_executable(${test} "doctest/doctest.cpp" "${found_file}")
        target_include_directories(${test} PRIVATE
            .
            $<TARGET_PROPERTY:DgeX::Lib,INCLUDE_DIRECTORIES>
        )
        target_link_libraries(${test} DgeX_Static)

        add_test(NAME ${test} COMMAND ${test})
    else()
        message(FATAL_ERROR "Test case ${test} not found")
    endif()
endforeach(test)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/Resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
