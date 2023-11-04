# Settings for deploying a release build.
#
# Preloads the cache with this settings when CMake is invoked directly from the command line.
#
# For details refer to: 
#    - The root CMakeLists file.
#    - https://stackoverflow.com/questions/17597673/cmake-preload-script-for-cache

## Configuration variables #############################################################################################
set(CFG_LIB_BUILD_TYPE "STATIC" CACHE STRING "" FORCE)
set(CFG_TAG "OFF" CACHE STRING "" FORCE)
set(CFG_TESTS_ENABLE_COVERAGE "ON" CACHE STRING "" FORCE)
set(CFG_CI "OFF" CACHE STRING "" FORCE)
set(BUILD_TESTING TRUE CACHE BOOL "" FORCE)

## Other ###############################################################################################################
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
set(CMAKE_GENERATOR "Ninja" CACHE STRING "" FORCE)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/other/cmake/gnu_toolchain.cmake" CACHE STRING "" FORCE)
