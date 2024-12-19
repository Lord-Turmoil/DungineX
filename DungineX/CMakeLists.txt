# ====================================================================
# DungineX Library
# ====================================================================

project(DungineXLib)

set(TARGET_NAME ${PROJECT_NAME})

# --------------------------------------------------------------------
# Source file
# --------------------------------------------------------------------
file(GLOB_RECURSE SRC_LIST CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_library(${TARGET_NAME} ${SRC_LIST})

if(DGEX_OPENGL)
    target_compile_definitions(${TARGET_NAME} PUBLIC DGEX_OPENGL)
else()
    message(FATAL_ERROR "Only OpenGL is supported at the moment")
endif()

if(DGEX_ENABLE_ASSERTION)
    target_compile_definitions(${TARGET_NAME} PUBLIC DGEX_ENABLE_ASSERT)
else()
    message(WARNING "Assertion disabled for DungineX")
endif()

# --------------------------------------------------------------------
# Include directories
# --------------------------------------------------------------------
target_include_directories(
    ${TARGET_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src
)

# --------------------------------------------------------------------
# Emits the library name to the parent scope
# --------------------------------------------------------------------
set(DGEX_LIB ${TARGET_NAME} PARENT_SCOPE)
