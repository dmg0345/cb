# Create library target for the test harness, always static.
add_library(cmocka STATIC)

# Fetch contents if not already present.
git_shallow_clone(
    cmocka
    "https://gitlab.com/cmocka/cmocka.git"
    "cmocka-1.1.7"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmocka"
)

## Compile Definitions #################################################################################################
target_compile_definitions(cmocka PRIVATE
    # Header files availability.
    "HAVE_ASSERT_H=1"
    "HAVE_DLFCN_H=1"
    "HAVE_INTTYPES_H=1"
    # "HAVE_IO_H=0"
    "HAVE_MALLOC_H=1"
    "HAVE_MEMORY_H=1"
    "HAVE_SETJMP_H=1"
    "HAVE_SIGNAL_H=1"
    "HAVE_STDARG_H=1"
    "HAVE_STDDEF_H=1"
    "HAVE_STDINT_H=1"
    "HAVE_STDIO_H=1"
    "HAVE_STDLIB_H=1"
    "HAVE_STRINGS_H=1"
    "HAVE_STRING_H=1"
    # "HAVE_SYS_STAT_H=0"
    # "HAVE_SYS_TYPES_H=0"
    "HAVE_TIME_H=1"
    "HAVE_UNISTD_H=1"

    # Structs availability.
    # "HAVE_STRUCT_TIMESPEC=0"

    # Functions availability.
    "HAVE_CALLOC=1"
    "HAVE_EXIT=1"
    # "HAVE_FPRINTF=0"
    # "HAVE_SNPRINTF=0"
    # "HAVE__SNPRINTF=0"
    # "HAVE__SNPRINTF_S=0"
    # "HAVE_VSNPRINTF=0"
    # "HAVE__VSNPRINTF=0"
    # "HAVE__VSNPRINTF_S=0"
    "HAVE_FREE=1"
    "HAVE_LONGJMP=1"
    # "HAVE_SIGLONGJMP=0"
    "HAVE_MALLOC=1"
    "HAVE_MEMCPY=1"
    "HAVE_MEMSET=1"
    "HAVE_PRINTF=1"
    "HAVE_SETJMP=1"
    "HAVE_SIGNAL=1"
    "HAVE_STRCMP=1"
    "HAVE_STRCPY=1"
    # "HAVE_VSNPRINTF=0"
    "HAVE_STRSIGNAL=1"
    "HAVE_CLOCK_GETTIME=1"

    # Endianness.
    "WORDS_SIZEOF_VOID_P=4"
    # "WORDS_BIGENDIAN=0"
)

## Sources #############################################################################################################
# Collect sources and add them to target.
set(SOURCES_CMOCKA
    "${CMAKE_CURRENT_SOURCE_DIR}/cmocka/src/cmocka.c"
    CACHE INTERNAL ""
)
target_sources(cmocka PRIVATE ${SOURCES_CMOCKA})

# Omit warnings in these files as they are third-party.
set_source_files_properties(${SOURCES_CMOCKA} TARGET_DIRECTORY cmocka PROPERTIES
    COMPILE_FLAGS "-Wno-sign-conversion -Wno-conversion"
)

## Includes ############################################################################################################
# Collect public directories, visible for both cmocka and the tests.
set(INCLUDE_DIRS_CMOCKA
    "${CMAKE_CURRENT_SOURCE_DIR}/cmocka/include"
    CACHE INTERNAL ""
)
target_include_directories(cmocka PUBLIC ${INCLUDE_DIRS_CMOCKA})
