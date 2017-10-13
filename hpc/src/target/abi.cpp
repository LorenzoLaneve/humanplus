// => src/target/abi.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/target/abi.h>
#include <hpc/ast/types/type.h>

#include <sstream>

using namespace hpc;

target::TargetABI *target::TargetABI::createABI(opts::TargetOptions &opts) {
    return new TargetABI(opts, CXXGenericARM); // FIXME
}

bool target::TargetABI::isCXXABI() const {
    switch (kind) {
        case CXXGenericItanium:
        case CXXGenericARM:
        case CXXiOS:
        case CXXiOS64:
        case CXXWatchOS:
        case CXXGenericAArch64:
        case CXXGenericMIPS:
        case CXXWebAssembly:
        case CXXMicrosoft32:
        case CXXMicrosoft64:
            return true;
    }
    
    llvm_unreachable("Invalid ABI.");
}

bool target::TargetABI::isItaniumABI() const {
    switch (kind) {
        case CXXGenericItanium:
        case CXXGenericARM:
        case CXXiOS:
        case CXXiOS64:
        case CXXWatchOS:
        case CXXGenericAArch64:
        case CXXGenericMIPS:
        case CXXWebAssembly:
            return true;
            
        case CXXMicrosoft32:
        case CXXMicrosoft64:
            return false;
    }
    
    llvm_unreachable("Invalid ABI.");
}

bool target::TargetABI::isMicrosoftABI() const {
    switch (kind) {
        case CXXGenericItanium:
        case CXXGenericARM:
        case CXXiOS:
        case CXXiOS64:
        case CXXWatchOS:
        case CXXGenericAArch64:
        case CXXGenericMIPS:
        case CXXWebAssembly:
            return false;
            
        case CXXMicrosoft32:
        case CXXMicrosoft64:
            return true;
    }
    
    llvm_unreachable("Invalid ABI.");
}


