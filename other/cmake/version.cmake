########################################################################################################################
# This file provides versioning information, including Git commit and similar values if in a repository.
########################################################################################################################

## Project Versioning ##################################################################################################
set(PROJECT_AUTHOR "Diego Martínez García")
set(PROJECT_CONTACT "dmg0345@gmail.com")
set(PROJECT_URL "https://github.com/dmg0345/cb")
set(PROJECT_DESCRIPTION "Circular Buffer")
set(PROJECT_COPYRIGHT "github.com/dmg0345/cb/blob/master/LICENSE")
set(PROJECT_VERSION_MAJOR "1")
set(PROJECT_VERSION_MINOR "0")
set(PROJECT_VERSION_PATCH "0")
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
string(TIMESTAMP PROJECT_BUILD_TIMESTAMP_UTC "%d-%m-%Y %H:%M:%S (UTC)" UTC)

## Git Versioning ######################################################################################################
# Run Git command to fetch results.
execute_process(COMMAND git rev-parse HEAD
                OUTPUT_VARIABLE PROJECT_VCS_HASH
                ERROR_QUIET
                OUTPUT_STRIP_TRAILING_WHITESPACE
                WORKING_DIRECTORY "${PROJECT_ROOT_DIR}")

if ("${PROJECT_VCS_HASH}" STREQUAL "")
    # Not a git repository (e.g. zipped file)
    set(PROJECT_VCS_HASH "N/A")
    set(PROJECT_VCS_TAG "N/A")
    set(PROJECT_VCS_BRANCH "N/A")
else()
    # Append plus sign if there are working changes.
    execute_process(COMMAND git status --porcelain=v1 --ignore-submodules
                    OUTPUT_VARIABLE GIT_OUTPUT
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    WORKING_DIRECTORY "${PROJECT_ROOT_DIR}")
    if (NOT "${GIT_OUTPUT}" STREQUAL "")
        set(PROJECT_VCS_HASH "+++${PROJECT_VCS_HASH}+++")
    endif()

    # Get latest tag in branch.
    execute_process(COMMAND git describe --tags
                    OUTPUT_VARIABLE PROJECT_VCS_TAG
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    WORKING_DIRECTORY "${PROJECT_ROOT_DIR}")

    # Get branch name.
    execute_process(COMMAND git branch --show-current
                    OUTPUT_VARIABLE PROJECT_VCS_BRANCH
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    WORKING_DIRECTORY "${PROJECT_ROOT_DIR}")
endif()

## Version File ########################################################################################################
# Create versioning file contents.
set(VERSION_INFO "/**
 ***********************************************************************************************************************
 * @file        version.h
 * @author      ${PROJECT_AUTHOR} (${PROJECT_CONTACT})
 * @date        ${PROJECT_BUILD_TIMESTAMP_UTC}
 * @version     ${PROJECT_VERSION}
 * @copyright   ${PROJECT_COPYRIGHT}
 ***********************************************************************************************************************
 */

// clang-format off
// THIS FILE IS AUTO-GENERATED, IF MODIFIED, CHANGES WILL BE OVERWRITTEN.

/* Define to prevent recursive inclusion -----------------------------------------------------------------------------*/
#ifndef CB_VERSION_H
#define CB_VERSION_H

#ifdef __cplusplus
extern \"C\" {
#endif

/** @defgroup cb_version_defs Definitions */
/** @defgroup cb_version_papi Public API */

/* Includes ----------------------------------------------------------------------------------------------------------*/
/* Exported types ----------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_version_defs
 * @{
 */

/**
 * @}
 */

/* Exported constants ------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_version_defs
 * @{
 */

/** 
 * @rst
 * ${PROJECT_AUTHOR}
 * @endrst
 */
#define ${PROJECT_NAME}_AUTHOR \"${PROJECT_AUTHOR}\"

/** 
 * @rst
 * ${PROJECT_CONTACT}
 * @endrst
 */
#define ${PROJECT_NAME}_CONTACT \"${PROJECT_CONTACT}\"

/** 
 * @rst
 * ${PROJECT_URL}
 * @endrst
 */
#define ${PROJECT_NAME}_URL \"${PROJECT_URL}\"

/** 
 * @rst
 * ${CMAKE_BUILD_TYPE}
 * @endrst
 */
#define ${PROJECT_NAME}_BUILD \"${CMAKE_BUILD_TYPE}\"

/** 
 * @rst
 * ${PROJECT_DESCRIPTION}
 * @endrst
 */
#define ${PROJECT_NAME}_DESCRIPTION \"${PROJECT_DESCRIPTION}\"

/** 
 * @rst
 * ${PROJECT_VERSION}
 * @endrst
 */
#define ${PROJECT_NAME}_VERSION \"${PROJECT_VERSION}\"

/**
 * @}
 */

/** 
 * @rst
 * ${PROJECT_VCS_HASH}
 * @endrst
 * @note The hash is enclosed in @e +++ characters if the project was built with uncommited changes.
 */
#define ${PROJECT_NAME}_COMMIT_HASH \"${PROJECT_VCS_HASH}\"

/** 
 * @rst
 * ${PROJECT_VCS_TAG}
 * @endrst
 */
#define ${PROJECT_NAME}_TAG \"${PROJECT_VCS_TAG}\"

/** 
 * @rst
 * ${PROJECT_VCS_BRANCH}
 * @endrst
 */
#define ${PROJECT_NAME}_BRANCH \"${PROJECT_VCS_BRANCH}\"

/** 
 * @rst
 * ${PROJECT_BUILD_TIMESTAMP_UTC}
 * @endrst
 */
#define ${PROJECT_NAME}_BUILD_TIMESTAMP_UTC \"${PROJECT_BUILD_TIMESTAMP_UTC}\"

/* Exported macro ----------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_version_papi
 * @{
 */

/**
 * @}
 */

/* Exported functions ------------------------------------------------------------------------------------------------*/
/**
 * @addtogroup cb_version_papi
 * @{
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CB_VERSION_H */

/******************************************************************************************************END OF FILE*****/")

# Store version contents to file.
set(PROJECT_VERSION_FILE_LOCATION "${PROJECT_ROOT_DIR}/src/inc/cb/other/version.h")
message(STATUS "Storing project version file list at '${PROJECT_VERSION_FILE_LOCATION}'...")
file(WRITE "${PROJECT_VERSION_FILE_LOCATION}" "${VERSION_INFO}")
configure_file("${PROJECT_VERSION_FILE_LOCATION}" "${PROJECT_VERSION_FILE_LOCATION}")
