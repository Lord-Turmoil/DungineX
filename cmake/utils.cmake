# Get DungineX version from DungineX/include/DgeX/Common/Macros.h" and 
# put it in DGEX_VERSION
function(dgex_extract_version)
    file(READ "${CMAKE_CURRENT_LIST_DIR}/DungineX/include/DgeX/Common/Macros.h" file_contents)
    string(REGEX MATCH "DGEX_VERSION_MAJOR ([0-9]+)" _ "${file_contents}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract major version number")
    endif()
    set(ver_major ${CMAKE_MATCH_1})

    string(REGEX MATCH "DGEX_VERSION_MINOR ([0-9]+)" _ "${file_contents}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract minor version number")
    endif()

    set(ver_minor ${CMAKE_MATCH_1})
    string(REGEX MATCH "DGEX_VERSION_PATCH ([0-9]+)" _ "${file_contents}")
    if(NOT CMAKE_MATCH_COUNT EQUAL 1)
        message(FATAL_ERROR "Could not extract patch version number")
    endif()
    set(ver_patch ${CMAKE_MATCH_1})

    set(DGEX_VERSION_MAJOR ${ver_major} PARENT_SCOPE)
    set(DGEX_VERSION_MINOR ${ver_minor} PARENT_SCOPE)
    set(DGEX_VERSION_PATCH ${ver_patch} PARENT_SCOPE)
    set(DGEX_VERSION "${ver_major}.${ver_minor}.${ver_patch}" PARENT_SCOPE)
endfunction()

# Turn on warnings on the given target
function(dgex_enable_warnings target_name)
    target_compile_options(
        ${target_name}
        PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
                -Wall
                -Wextra
                -Wconversion
                -pedantic
                -Werror
                -Wfatal-errors>
                $<$<CXX_COMPILER_ID:MSVC>:/W4>)
endfunction()

# Turn off warnings on the given target 
function(dgex_disable_warnings target_name)
    get_target_property(type ${target_name} TYPE)
    if (NOT ${type} STREQUAL "INTERFACE_LIBRARY")
        target_compile_options(
            ${target_name}
            PRIVATE $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
                    -w>
                    $<$<CXX_COMPILER_ID:MSVC>:/W0>)
    endif()
endfunction()

# Disable CRT secure warnings on the given target
function(dgex_disable_crt_warnings target_name)
    get_target_property(type ${target_name} TYPE)
    if (${type} STREQUAL "INTERFACE_LIBRARY")
        target_compile_definitions(${target_name} INTERFACE _CRT_SECURE_NO_WARNINGS)
    else()
        target_compile_definitions(${target_name} PRIVATE _CRT_SECURE_NO_WARNINGS)
    endif()
endfunction()
