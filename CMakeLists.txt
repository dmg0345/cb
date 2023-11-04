########################################################################################################################
# Root CMakeLists file, for documentation refer to:
#
# > https://cmake.org/cmake/help/v3.25/
########################################################################################################################
# Set minimum version for CMake.
cmake_minimum_required(VERSION 3.25)

# Enable or disable verbosity, useful for debugging. Use it as a cache variable so it applies to all files.
option(CMAKE_VERBOSE_MAKEFILE "Set to ON to enable verbosity, defaults to 'OFF'." OFF)
# Allow the generation of 'compile_commands.json' for debugging purposes and intellisense.
option(CMAKE_EXPORT_COMPILE_COMMANDS "Set to ON to generate compilation database, defaults to 'OFF'." ON)

## Policies ############################################################################################################
# Set policy to convert relative paths in target_sources command to absolute paths.
cmake_policy(SET CMP0076 NEW)

## Project Definition ##################################################################################################
# Define the project name.
project(CB)
# Load utils.
include("other/cmake/utils.cmake")
# Include FetchContent API.
include(FetchContent)

# Enable languages and configure them explicitly.
enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Enable testing with 'ctest' and create necessary configuration files, do not create UTILITY targets.
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
include(CTest)

# Set the project root the current working directory at this point, that is, the root of the repository.
set(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

## Project Options #####################################################################################################
# The type of build to perform with the libraries.
set(CFG_LIB_BUILD_TYPE "STATIC" CACHE STRING "Library build type, defaults to 'STATIC'.")
set_property(CACHE CFG_LIB_BUILD_TYPE PROPERTY STRINGS "STATIC" "SHARED")

# Defines whether the files should be tagged prior to a build.
set(CFG_TAG "OFF" CACHE STRING "Tags the C/C++ source and header files, defaults to 'OFF'.")
set_property(CACHE CFG_TAG PROPERTY STRINGS "OFF" "FILENAMES_ONLY" "ON")

# Adds coverage to the tests.
set(CFG_TESTS_ENABLE_COVERAGE "OFF" CACHE STRING "Adds coverage to the off-target tests, defaults to 'OFF'.")
set_property(CACHE CFG_TESTS_ENABLE_COVERAGE PROPERTY STRINGS "OFF" "ON")

# Other project configuration variables:
#
# - CFG_CI
#    Boolean variable that indicates if running in CI, if set to ON, or not, if set to OFF.
#
# - BUILD_TESTING
#    Boolean variable that can be ON or OFF, see: https://cmake.org/cmake/help/latest/module/CTest.html.

# After all variables have been defined generate versioning files and variables.
include("other/cmake/version.cmake")

# Print a summary of relevant variables.
message(STATUS "########## Relevant CMake Configuration Values ##########")
message(STATUS "CMAKE_BUILD_TYPE: '${CMAKE_BUILD_TYPE}'")
message(STATUS "CMAKE_GENERATOR: '${CMAKE_GENERATOR}'")
message(STATUS "CMAKE_TOOLCHAIN_FILE: '${CMAKE_TOOLCHAIN_FILE}'")
message(STATUS "CFG_LIB_BUILD_TYPE: '${CFG_LIB_BUILD_TYPE}'")
message(STATUS "CFG_TAG: '${CFG_TAG}'")
message(STATUS "CFG_TESTS_ENABLE_COVERAGE: '${CFG_TESTS_ENABLE_COVERAGE}'")
message(STATUS "CFG_CI: '${CFG_CI}'")
message(STATUS "BUILD_TESTING: '${BUILD_TESTING}'")
message(STATUS "CMAKE_VERBOSE_MAKEFILE: '${CMAKE_VERBOSE_MAKEFILE}'")
message(STATUS "#########################################################")

# Perform sanity check on testing related variables:
#  - Coverage can only be enabled when the tests are built.
if(NOT ${BUILD_TESTING})
    if ((${CFG_TESTS_ENABLE_COVERAGE} STREQUAL "ON"))
        message(FATAL_ERROR "If CFG_TESTS_ENABLE_COVERAGE is 'ON' BUILD_TESTING needs to be 'ON'.")
    endif()
endif()

## Installation directories ############################################################################################
# Override default installation directory if one was not provided by the user.
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_ROOT_DIR}/dist" CACHE PATH "Installation directory" FORCE)
endif()

# Folder where executables and shared libraries and installed.
set(CMAKE_INSTALL_RUNTIMEDIR "${CMAKE_INSTALL_PREFIX}/bin")
# Folder where libraries are installed.
set(CMAKE_INSTALL_LIBDIR "${CMAKE_INSTALL_PREFIX}/lib")
# Folder where include files are installed.
set(CMAKE_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_PREFIX}/inc")

## Compile time macros #################################################################################################
# Add DEBUG or RELEASE macros to the build so the build type can be identified from the code.
if((${CMAKE_BUILD_TYPE} STREQUAL "Debug"))
    add_compile_definitions("DEBUG")
elseif((${CMAKE_BUILD_TYPE} STREQUAL "Release"))
    add_compile_definitions("RELEASE")
endif()

## Compile time flags ##################################################################################################
# Handle DEBUG release flags for the C compiler:
# -Og: Optimize for debugging experience rather than speed or size.
# -ggdb3: Generate highest level debug information in GDB extended format.
string(APPEND CMAKE_C_FLAGS_DEBUG " -Og" " -ggdb3")
# Handle DEBUG release flags for the C++ compiler:
# -Og: Optimize for debugging experience rather than speed or size.
# -ggdb3: Generate highest level debug information in GDB extended format.
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -Og" " -ggdb3")
# Handle DEBUG release flags for the assembler compiler:
# -ggdb3: Generate debug information in default format.
string(APPEND CMAKE_ASM_FLAGS_DEBUG " -ggdb3")

# Handle RELEASE release flags for the C compiler:
# -O3: Enable all optimizations.
# -ggdb0: Produce no debug information.
# -flto: Enable Link Time Optimization.
string(APPEND CMAKE_C_FLAGS_RELEASE " -O3" " -ggdb0" " -flto")
# Handle RELEASE release flags for the C++ compiler:
# -O3: Enable all optimizations.
# -ggdb0: Produce no debug information.
# -flto: Enable Link Time Optimization.
string(APPEND CMAKE_CXX_FLAGS_RELEASE " -O3" " -ggdb0" " -flto")
# Handle RELEASE release flags for the assembler compiler:
# -ggdb0: Produce no debug information.
string(APPEND CMAKE_ASM_FLAGS_RELEASE " -ggdb0")

## Tag and build of files ##############################################################################################
# Check if tagging files.
if(NOT (${CFG_TAG} STREQUAL "OFF"))
    include("other/cmake/tag.cmake")
endif()

# Add application.
add_subdirectory("src")
# Add tests if testing is enabled.
if(BUILD_TESTING)
    add_subdirectory("tests")
endif()
