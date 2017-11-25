# => CMakeLists.txt (main)
#
#                     The Human Plus Project
#
# This file is distributed under the University of Illinois/NCSA
# Open Source License. See LICENSE.TXT for details.
#
#

cmake_minimum_required(VERSION 3.4.3)

# Project should be built as different standalone projects?
if(NOT HUMANPLUS_GEN_STANDALONE_PROJECTS)
    project(HumanPlus VERSION 0.9 LANGUAGES C CXX ASM-NASM)
    set(HUMANPLUS_GEN_AS_ONE_PROJECT ON)
endif()

set(CMAKE_CXX_STANDARD 11)

if(NOT DEFINED HUMANPLUS_BUILD_HPC)
    set(HUMANPLUS_BUILD_HPC ON)
endif()

if(NOT DEFINED HUMANPLUS_BUILD_HUMANLOGIC)
    set(HUMANPLUS_BUILD_HUMANLOGIC ON)
endif()

# Target: hpc
if(HUMANPLUS_BUILD_HPC)
    message(STATUS "Making hpc...")
    include(${CMAKE_CURRENT_SOURCE_DIR}/hpc/CMakeLists.txt)
    message(STATUS "Making hpc... - done")
endif()

# Target: humanlogic
if(HUMANPLUS_BUILD_HUMANLOGIC)
    message(STATUS "Making humanlogic...")
    include(${CMAKE_CURRENT_SOURCE_DIR}/humanlogic/CMakeLists.txt)
    message(STATUS "Making humanlogic... - done")
endif()
