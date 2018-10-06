# => hpc/CMakeLists.txt
#
#                     The Human Plus Project
#
# This file is distributed under the University of Illinois/NCSA
# Open Source License. See LICENSE.TXT for details.
#
#

cmake_minimum_required(VERSION 3.4.3)

# Target: hpc
if(NOT HUMANPLUS_GEN_AS_ONE_PROJECT) # If projects should be generated as standalone projects, declare the project.
    project(hpc VERSION 0.9 LANGUAGES C CXX ASM)
    set(CMAKE_CXX_STANDARD 11)
endif()

message(STATUS "Detecting LLVM...")

# Importing LLVM
set(LLVM_DIR "${LLVM_BUILD_DIR}/lib/cmake/llvm")
find_package(LLVM QUIET CONFIG)
if(NOT LLVM_FOUND)
    message(FATAL_ERROR "hpc needs LLVM 5.x to be compiled.
You can download LLVM from the project site: http://llvm.org
Build LLVM with CMake and pass LLVM build directory to Human Plus by setting LLVM_BUILD_DIR.")
endif()

message(STATUS "Detecting LLVM... - found")

message(STATUS "Found LLVM (version ${LLVM_PACKAGE_VERSION})")
if(NOT ((${LLVM_PACKAGE_VERSION} VERSION_GREATER "5.0.0" AND
	${LLVM_PACKAGE_VERSION} VERSION_LESS "6.0.0") OR
	${LLVM_PACKAGE_VERSION} VERSION_EQUAL "5.0.0"))
    message(FATAL_ERROR "The specified LLVM build in ${LLVM_BUILD_DIR} is too old or new (version ${LLVM_PACKAGE_VERSION}). hpc needs LLVM 5.x")
endif()

message(STATUS "Using LLVM build in ${LLVM_BUILD_DIR}")


if(HUMANPLUS_GEN_AS_ONE_PROJECT) # if we need to generate one project then the folder we're working on is the main directory
    set(TARG_DIR "hpc/")
else() # this is standalone project and we're already in hpc/ directory
    set(TARG_DIR "")
endif()

include_directories(${LLVM_INCLUDE_DIRS} "${TARG_DIR}include")
add_definitions(${LLVM_DEFINITIONS})


# using GLOB_RECURSE is a bad use and will be removed soon.
file(GLOB_RECURSE HUMANPLUS_HPC_SOURCE_FILES
    "${TARG_DIR}src/*.c"
    "${TARG_DIR}src/*.cpp"
)

add_executable(hpc ${HUMANPLUS_HPC_SOURCE_FILES})


function(hpc_take_targets TARGET_NAME TARGET_FLAG)
    foreach(TARGET_COMPONENT ${ARGN})
        llvm_map_components_to_libnames(COMPONENT_LIBS "${TARGET_FLAG}${TARGET_COMPONENT}")
        set(TARGET_LIBS ${TARGET_LIBS} ${COMPONENT_LIBS})
    endforeach()
    set(HPC_LLVM_LIBS ${HPC_LLVM_LIBS} ${TARGET_LIBS} PARENT_SCOPE)
    message(STATUS "Added ${TARGET_NAME} target.")
endfunction()


llvm_map_components_to_libnames(HPC_LLVM_LIBS core support option debuginfopdb debuginfodwarf debuginfocodeview lto symbolize mirparser libdriver tablegen asmparser asmprinter mc mcparser mcdisassembler mcjit passes transformutils ipo instcombine vectorize irreader linker interpreter codegen scalaropts instrumentation profiledata bitreader bitwriter orcjit executionengine target analysis runtimedyld object selectiondag)

hpc_take_targets("AArch64" aarch64   asmparser asmprinter codegen desc disassembler info utils)
hpc_take_targets("AMDGPU" amdgpu     asmparser asmprinter codegen desc disassembler info utils)
hpc_take_targets("ARM" arm           asmparser asmprinter codegen desc disassembler info)
hpc_take_targets("BPF" bpf           asmprinter codegen desc disassembler info)
hpc_take_targets("Hexagon" hexagon   asmparser codegen desc disassembler info)
hpc_take_targets("Lanai" lanai       asmparser asmprinter codegen desc disassembler info)
hpc_take_targets("MIPS" mips         asmparser asmprinter codegen desc disassembler info)
hpc_take_targets("MSP430" msp430     asmprinter codegen desc info)
hpc_take_targets("NVPTX" nvptx       asmprinter codegen desc info)
hpc_take_targets("PowerPC" powerpc   asmparser asmprinter codegen desc disassembler info)
hpc_take_targets("SPARC" sparc       asmparser asmprinter codegen desc disassembler info)
hpc_take_targets("System Z" systemz  asmparser asmprinter codegen desc disassembler info)
hpc_take_targets("x86" x86           asmparser asmprinter codegen desc disassembler info utils)
hpc_take_targets("XCore" xcore       asmprinter codegen desc disassembler info)


target_link_libraries(hpc ${HPC_LLVM_LIBS})

install(TARGETS hpc
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
