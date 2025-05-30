# ====================================================================
# Utility functions for DungineX CMake configuration
# ====================================================================

# Turn on warnings on the given target
function(dgex_enable_strict_warnings target_name)
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

# Get all dependencies of a target and store them in a result variable.
function(dgex_get_dependencies target_name output)
    # Initialize the result variable.
    set(dependencies ${target_name})
    
    # Iterate over the dependencies of the target.
    foreach(dependency ${dependencies})
        get_target_property(direct_dependencies ${dependency} INTERFACE_LINK_LIBRARIES)
        if(direct_dependencies)
            foreach(direct_dependency ${direct_dependencies}) 
                # Avoid duplicates and ensure the dependency is a valid target.
                if(TARGET ${direct_dependency} AND NOT ${direct_dependency} IN_LIST dependencies)
                    list(APPEND dependencies ${direct_dependency})
                    # Get indirect dependencies recursively.
                    dgex_get_dependencies(${direct_dependency} indirect_dependencies)
                    foreach(indirect_dependency ${indirect_dependencies})
                        if(NOT indirect_dependency IN_LIST dependencies)
                            list(APPEND dependencies ${indirect_dependency})
                        endif()
                    endforeach(indirect_dependency)
                endif()
            endforeach(direct_dependency)
        endif()
    endforeach(dependency)
                
    # Return the result
    set(${output} ${dependencies} PARENT_SCOPE)
endfunction()

# Get the target name from a given alias.
function(dgex_get_target_name alias_name output)
    get_target_property(target_name ${alias_name} ALIASED_TARGET)
    if("${target_name}" STREQUAL "target_name-NOTFOUND")
        set(${output} ${alias_name} PARENT_SCOPE)
    else()
        set(${output} ${target_name} PARENT_SCOPE)
    endif()
endfunction()
