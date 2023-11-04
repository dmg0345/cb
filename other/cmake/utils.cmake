########################################################################################################################
# Generic utilities and functionality for CMake.
########################################################################################################################

# @brief Converts a boolean to integer.
# @param[in] bool_var The name of the variable with the boolean value.
# @param[out] int_var The name of the variable where the resulting integer will be stored.
# @example bool2int(BOOL_VARIABLE INT_VARIABLE)
function(bool2int bool_var int_var)
    if(${bool_var})
        set(${int_var} 1 PARENT_SCOPE)
    else()
        set(${int_var} 0 PARENT_SCOPE)
    endif()
endfunction()

# @brief Downloads a tag/branch in a Git repository to a source directory if it doesn't exist.
# @param[in] identifier A name or identifier to the process.
# @param[in] git_repository The repository to clone.
# @param[in] git_tag The tag or branch to clone.
# @param[in] source_dir The directory where to clone the repository.
# @example git_clone(cmocka https://gitlab.com/cmocka/cmocka.git cmocka-1.1.7 "${CMAKE_CURRENT_SOURCE_DIR}/cmocka")
# @note Requires FetchContent API, add ``include(FetchContent)`` in your *CMakeLists* file.
function(git_shallow_clone identifier git_repository git_tag source_dir)
    # Check if the directory already exists, in which case, skip it.
    if(EXISTS "${source_dir}")
        message(STATUS "Not cloning '${identifier}' as source directory '${source_dir}' already exists...")
    else()
        message(STATUS "Cloning '${identifier}'...")

        # Checkout tag/branch and clone submodules recursively if any, clone with a random string so that
        # there are no conflicts with FindPackage.
        string(RANDOM LENGTH 10 rand_string)
        FetchContent_Declare(
            "${identifier}_${rand_string}"
            GIT_REPOSITORY "${git_repository}"
            GIT_TAG "${git_tag}"
            GIT_SHALLOW ON
            GIT_PROGRESS ON
            GIT_SUBMODULES_RECURSE ON
            SOURCE_DIR "${source_dir}"
            EXCLUDE_FROM_ALL
            OVERRIDE_FIND_PACKAGE
        )

        # Populate contents, without running any configure, build steps if presend in the project.
        message(STATUS "Populating '${source_dir}' with '${identifier}'...")
        FetchContent_Populate("${identifier}_${rand_string}")
    endif()
endfunction()
