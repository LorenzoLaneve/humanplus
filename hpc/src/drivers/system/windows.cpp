// => src/drivers/system/windows.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/drivers/system/windows.h>

using namespace hpc;

std::string drivers::WindowsDriver::defaultAssemblerOutputName() {
    return "a.exe";
}
