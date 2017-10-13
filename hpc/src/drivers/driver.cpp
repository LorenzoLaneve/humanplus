// => src/drivers/driver.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/drivers/driver.h>

using namespace hpc;

drivers::Driver::Driver(const char **argBegin, const char **argEnd, const char **argv0, void *main)
: args(llvm::makeArrayRef(argBegin, argEnd)), argv0(argv0), main(main) {  }
