# ====================================================================
# DungineX Vendor Libraries
# ====================================================================

# --------------------------------------------------------------------
# Adding SDL
# --------------------------------------------------------------------

if(NOT DGEX_USE_SHARED_SDL)
    set(BUILD_SHARED_LIBS OFF)
endif()

# Use EXCLUDE_FROM_ALL so that DSL's install targets and configs don't
# pollute upwards into our configuration.
add_subdirectory(SDL EXCLUDE_FROM_ALL)

# --------------------------------------------------------------------
# Adding SDL_ttf for font support
# --------------------------------------------------------------------

# Tell SDL_ttf to build its own dependencies.
set(SDLTTF_VENDORED ON)
add_subdirectory(SDL_ttf EXCLUDE_FROM_ALL)

# --------------------------------------------------------------------
# Adding SDL_image to load various image formats
# --------------------------------------------------------------------

# Tell SDL_image to build its own dependencies.
set(SDLIMAGE_VENDORED ON)

# Explicitly add the image formats we want to support.
set(SDLIMAGE_AVIF OFF)
set(SDLIMAGE_BMP  OFF)
set(SDLIMAGE_GIF  OFF)
set(SDLIMAGE_JPG  OFF)
set(SDLIMAGE_JXL  OFF)
set(SDLIMAGE_LBM  OFF)
set(SDLIMAGE_PCX  OFF)
set(SDLIMAGE_PNG  ON ) # png supported
set(SDLIMAGE_PNM  OFF)
set(SDLIMAGE_QOI  OFF)
set(SDLIMAGE_SVG  ON ) # svg supported
set(SDLIMAGE_TGA  OFF)
set(SDLIMAGE_TIF  OFF)
set(SDLIMAGE_WEBP OFF)
set(SDLIMAGE_XCF  OFF)
set(SDLIMAGE_XPM  OFF)
set(SDLIMAGE_XV   OFF)

add_subdirectory(SDL_image EXCLUDE_FROM_ALL)

# --------------------------------------------------------------------
# Adding spdlog for logging
# --------------------------------------------------------------------

add_subdirectory(spdlog)

# --------------------------------------------------------------------
# Creating all-in-one vendor dependency
# --------------------------------------------------------------------

# This is a convenience target that includes all the vendor.
add_library(DgeX_Vendor INTERFACE)

# Link all the vendor dependencies to the DgeX::Vendor.
target_link_libraries(DgeX_Vendor INTERFACE 
    SDL3_ttf::SDL3_ttf
    SDL3_image::SDL3_image
    SDL3::SDL3 # SDL must be the last item in the list.
    spdlog::spdlog
)

add_library(DgeX::Vendor ALIAS DgeX_Vendor)

# --------------------------------------------------------------------
# Disable Warnings
# --------------------------------------------------------------------

dgex_get_dependencies(DgeX::Vendor vendor_dependencies)
foreach(vendor ${vendor_dependencies})
    get_target_property(vendor_target ${vendor} ALIASED_TARGET)
    if("${vendor_target}" STREQUAL "vendor_target-NOTFOUND")
        dgex_disable_warnings(${vendor})
    else()
        dgex_disable_warnings(${vendor_target})
    endif()
endforeach()
