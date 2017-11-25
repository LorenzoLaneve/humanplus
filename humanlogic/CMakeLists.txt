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
endif()

file(GLOB_RECURSE HUMANPLUS_HUMANLOGIC_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "humanlogic/src/*.c"
)

add_library(humanlogic ${HUMANPLUS_HUMANLOGIC_SOURCE_FILES})
