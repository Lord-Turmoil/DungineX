# ====================================================================
# DungineX Library
# ====================================================================

include(cmake/version.cmake)

dgex_extract_version()

project(DungineX
    VERSION ${DGEX_VERSION}
    DESCRIPTION "A 2D game engine"
    LANGUAGES CXX
)

message(STATUS "Build DungineX: ${DGEX_VERSION}")
message(STATUS "Build DungineX as shared: ${DGEX_BUILD_SHARED}")
message(STATUS "Build DungineX as static: ${DGEX_BUILD_STATIC}")

# --------------------------------------------------------------------
# Project Configuration
# --------------------------------------------------------------------

set(TARGET_SHARED DgeX_Shared)
set(TARGET_STATIC DgeX_Static)
set(BINARY_NAME DgeX)

if(DGEX_USE_SHARED)
    generate_product_version(version_info
        NAME "DungineX"
        VERSION_MAJOR     ${DGEX_VERSION_MAJOR}
        VERSION_MINOR     ${DGEX_VERSION_MINOR}
        VERSION_PATCH     ${DGEX_VERSION_PATCH}
        VERSION_REVISION  ${DGEX_VERSION_REVISION}
        COMPANY_NAME      "New Desire Studios"
        # COMPANY_COPYRIGHT
        # COMMENTS
        ORIGINAL_FILENAME "${BINARY_NAME}.dll"
        # INTERNAL_NAME
        FILE_DESCRIPTION  "DungineX Game Engine Library"
    )
endif()

message(STATUS "Publishing DungineX: ${DGEX_PUBLISH}")

# --------------------------------------------------------------------
# Source Files
# --------------------------------------------------------------------

# Add .h to the sources so that Visual Studio can recognize them.
file(GLOB_RECURSE DGEX_SOURCES CONFIGURE_DEPENDS 
    "${PROJECT_SOURCE_DIR}/include/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

# --------------------------------------------------------------------
# Targets
# --------------------------------------------------------------------

# Shared and static targets have some common configurations.
macro(configure_dgex_target target_name)
    target_include_directories(
        ${target_name}
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
        PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src
    )
    target_link_libraries(${target_name} DgeX::Vendor)
    
    if(DGEX_STRICT_WARNINGS)
        dgex_enable_strict_warnings(${target_name})
    endif()

    if(DGEX_PUBLISH)
        target_compile_definitions(${target_name} PRIVATE DGEX_PUBLISH)
    endif()
    if(DGEX_ENABLE_ASSERT)
        target_compile_definitions(${target_name} PRIVATE DGEX_ENABLE_ASSERT)
    endif()
endmacro()

if(DGEX_BUILD_SHARED)
    add_library(${TARGET_SHARED} SHARED ${DGEX_SOURCES} ${version_info})
    configure_dgex_target(${TARGET_SHARED})

    # Shared library need some extra properties.
    set_target_properties(${TARGET_SHARED} PROPERTIES OUTPUT_NAME ${BINARY_NAME})
    set_target_properties(${TARGET_SHARED} PROPERTIES POSITION_INDEPENDENT_CODE ON)
    target_compile_definitions(${TARGET_SHARED} PRIVATE DGEX_EXPORT)
endif()

if(DGEX_BUILD_STATIC)
    add_library(${TARGET_STATIC} STATIC ${DGEX_SOURCES})
    configure_dgex_target(${TARGET_STATIC})
endif()

# --------------------------------------------------------------------
# Target Aliases
# --------------------------------------------------------------------

if(DGEX_USE_SHARED)
    add_library(DgeX::Lib ALIAS ${TARGET_SHARED})
else()
    add_library(DgeX::Lib ALIAS ${TARGET_STATIC})
endif()
