# ====================================================================
# minet core unit tests
# ====================================================================

set(dgex_tests IntrusiveList ObjectPool)

foreach(test ${dgex_tests})
    add_executable(${test} doctest.cpp "${test}Test.cpp")
    target_include_directories(${test} PRIVATE . $<TARGET_PROPERTY:${DGEX_LIB},INCLUDE_DIRECTORIES>)
    target_link_libraries(${test} ${DGEX_LIB})
    add_test(NAME ${test} COMMAND ${test})
endforeach()
