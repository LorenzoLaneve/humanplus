# => humanlogic/CMakeLists.txt
#
#                     The Human Plus Project
#
# This file is distributed under the University of Illinois/NCSA
# Open Source License. See LICENSE.TXT for details.
#
#

cmake_minimum_required(VERSION 3.4.3)

# Target: humanlogic
if(NOT HUMANPLUS_GEN_AS_ONE_PROJECT) # If projects should be generated as standalone projects, declare the project.
    project(humanlogic VERSION 0.9 LANGUAGES C CXX ASM)
    set(CMAKE_CXX_STANDARD 11)
endif()

if(HUMANPLUS_GEN_AS_ONE_PROJECT) # if we need to generate one project then the folder we're working on is the main directory
    set(TARG_DIR "humanlogic/")
else() # this is standalone project and we're already in humanlogic/ directory
    set(TARG_DIR "")
endif()

# using GLOB_RECURSE is a bad use and will be removed soon.
file(GLOB_RECURSE HUMANPLUS_HUMANLOGIC_SOURCE_FILES
    "${TARG_DIR}src/*.c"
)

add_library(humanlogic ${HUMANPLUS_HUMANLOGIC_SOURCE_FILES})

install(TARGETS humanlogic
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
