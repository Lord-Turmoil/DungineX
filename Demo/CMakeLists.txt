# ====================================================================
# DungineX Demo projects
# ====================================================================

set(DEMOS 
    AppDemo
    Car
    Firework
    Platform)

foreach(demo ${DEMOS})
    add_subdirectory(${demo})
endforeach()

set(DGEX_DEMOS ${DEMOS} PARENT_SCOPE)
