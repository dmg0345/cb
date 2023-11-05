########################################################################################################################
# This file covers the GNU toolchain, intended for off-target development, which can be found at:
#
# > https://gcc.gnu.org/gcc-12/
#
# Run the following commands to store the results to file to analyze them later:
#       > gcc --verbose --help > ./other/cmake/gnu_toolchain_opts_gcc.txt
#       > g++ --verbose --help > ./other/cmake/gnu_toolchain_opts_g++.txt
#       > ld --verbose --help > ./other/cmake/gnu_toolchain_opts_ld.txt
#       > as --verbose --help > ./other/cmake/gnu_toolchain_opts_as.txt
# Other relevant commands are:
#       > gcc --print-file-name=include
########################################################################################################################

## Target Definition ###################################################################################################
# Set the host system to build for the host.
set(CMAKE_SYSTEM_NAME "${CMAKE_HOST_SYSTEM_NAME}")
set(CMAKE_SYSTEM_PROCESSOR "${CMAKE_HOST_SYSTEM_PROCESSOR}")

## Toolchain Paths #####################################################################################################
# Set paths to tools.
# Note the assembler compiler is the C compiler, so that preprocessor directives in assembly files can be processed.
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_ASM_COMPILER "${CMAKE_C_COMPILER}")
set(CMAKE_OBJCOPY "/usr/bin/objcopy")
set(CMAKE_OBJDUMP "/usr/bin/objdump")
set(CMAKE_SIZE "/usr/bin/size")

# Set executable suffix explicitly.
set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")

## C/C++/ASM Compiler / Linker Options #################################################################################
# The general options include:
# -Wall: Enable most warning messages.
# -Werror: Treat warnings as errors.
# -Wextra: Enable extra warnings.
# -Wpedantic: Enable warnings required for strict compliance with standard.
# -Wconversion: Warn about implicit conversions that might change a value.
# -ffunction-sections: Place each function in its own section.
# -fdata-sections: Place data items in their own section.
# -Wl,--gc-sections: Linker, delete unused sections.
# -Wl,-Map mapfile.map: Linker, generate mapfile for each executable target called 'mapfile.map'.
# -isystem /usr/lib/gcc/x86_64-linux-gnu/12/include: Explicit folder for 'clangd' and other tools.
# -isystem /usr/include: Explicit folder for 'clangd' and other tools.
string(CONCAT FLAGS 
    " -Wall"
    " -Werror"
    " -Wextra"
    " -Wpedantic"
    " -Wconversion"
    " -ffunction-sections"
    " -fdata-sections"
    " -Wl,--gc-sections"
    " -Wl,-Map=mapfile.map"
    " -isystem /usr/lib/gcc/x86_64-linux-gnu/12/include"
    " -isystem /usr/include"
)

# The C specific options include:
string(CONCAT C_FLAGS "")

# The C++ specific options include:
string(CONCAT CXX_FLAGS "")

# General compiler options for the C compiler.
set(CMAKE_C_FLAGS "${FLAGS} ${C_FLAGS}")
# General compiler options for the C++ compiler.
set(CMAKE_CXX_FLAGS "${FLAGS} ${CXX_FLAGS}")
