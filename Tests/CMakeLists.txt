# ====================================================================
# minet core unit tests
# ====================================================================

set(dgex_tests
    # Library tests
    IntrusiveList
    ObjectPool
    # Style tests
    Dom
    Property
    Selector
)

foreach(test ${dgex_tests})
    set(test_target "${test}Test")
    add_executable(${test_target} doctest.cpp "${test}Test.cpp")
    target_include_directories(${test_target} PRIVATE . $<TARGET_PROPERTY:${DGEX_LIB},INCLUDE_DIRECTORIES>)
    target_link_libraries(${test_target} ${DGEX_LIB})
    add_test(
        NAME ${test}
        COMMAND ${test_target} 
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
endforeach()
