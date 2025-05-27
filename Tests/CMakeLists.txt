# ====================================================================
# DungineX Unit Tests
# ====================================================================

message(STATUS "Build DungineX unit tests")

set(tests Version)

foreach(test ${tests})
    add_executable(${test} "doctest/doctest.cpp" ${test}.cpp)
    target_include_directories(${test} PRIVATE . $<TARGET_PROPERTY:DgeX::Lib,INCLUDE_DIRECTORIES>)
    target_link_libraries(${test} DgeX_Static)

    add_test(NAME ${test} COMMAND ${test})
endforeach(test)
