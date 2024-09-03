# ====================================================================
# Dependencies of DungineX
# ====================================================================

set(vendors
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

foreach(vendor ${vendors})
    add_subdirectory(${vendor})
endforeach()

set(DGEX_VENDORS ${vendors} PARENT_SCOPE)
