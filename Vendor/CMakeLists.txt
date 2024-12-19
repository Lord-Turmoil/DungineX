# ====================================================================
# Dependencies of DungineX
# ====================================================================

set(VENDORS
    freetype
    glad
    GLFW
    glm
    lodepng
    msdf-atlas-gen
    msdfgen
    spdlog
    stb
    tinyxml2)

foreach(vendor ${VENDORS})
    add_subdirectory(${vendor})
endforeach()

set(DGEX_VENDORS ${VENDORS} PARENT_SCOPE)
