# ====================================================================
# DungineX Demo projects
# ====================================================================

set(demos 
    AppDemo
    Car
    Firework
    Platform)

foreach(demo ${demos})
    add_subdirectory(${demo})
endforeach()

set(DGEX_DEMOS ${demos} PARENT_SCOPE)
