# Create library target.
add_library(cb ${CFG_LIB_BUILD_TYPE})

# Set installation directories.
install(TARGETS cb
        RUNTIME DESTINATION "${CMAKE_INSTALL_RUNTIMEDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}")

# Add circular buffer.
add_subdirectory("cb")

## Source files ########################################################################################################
# Collect source files.
set(SOURCES_CB_ALL
    ${SOURCES_CB}
    CACHE INTERNAL ""
)
target_sources(cb PRIVATE ${SOURCES_CB_ALL})

## Includes ############################################################################################################
# Collect private include directories and add them to target, this will only be visible within the library.
set(INCLUDE_DIRS_CB_ALL
    ${INCLUDE_DIRS_CB}
    "${CMAKE_CURRENT_SOURCE_DIR}/inc"
    CACHE INTERNAL ""
)
target_include_directories(cb PRIVATE ${INCLUDE_DIRS_CB_ALL})

# Collect interface include directories and add them to target, these will only be visible by the application.
set(INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/inc"
)
target_include_directories(cb INTERFACE ${INCLUDE_DIRS})

## Distributable includes ##############################################################################################
# Variables to paths for convenience.
set(CB_SRC_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc/cb")
set(CB_INSTALL_ROOT_DIR "${CMAKE_INSTALL_INCLUDEDIR}/cb")

# TODO: Handle files
# # Common include files.
# install(FILES "${CB_SRC_ROOT_DIR}/cb.h" DESTINATION "${CB_INSTALL_ROOT_DIR}")
# install(FILES
#     "${CB_SRC_ROOT_DIR}/other/version.h"
#     "${CB_SRC_ROOT_DIR}/other/commons.h"
#     DESTINATION "${CB_INSTALL_ROOT_DIR}/other"
# )
# 
# # Interface files.
# install(FILES
#     "${BDE_SRC_ROOT_DIR}/cb/decode.h"
#     "${BDE_SRC_ROOT_DIR}/cb/encode.h"
#     "${BDE_SRC_ROOT_DIR}/cb/url_decode.h"
#     "${BDE_SRC_ROOT_DIR}/cb/url_encode.h"
#     DESTINATION "${BDE_INSTALL_ROOT_DIR}/cb"
# )
# 