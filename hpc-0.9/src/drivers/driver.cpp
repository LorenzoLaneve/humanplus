//
//  driver.cpp
//  HumanPlus
//
//  Created by Lore on 22/01/2017.
//  Copyright © 2017 Lore. All rights reserved.
//

#include <hpc/drivers/driver.h>

using namespace hpc;

drivers::Driver::Driver(const char **argBegin, const char **argEnd, const char **argv0, void *main)
: args(llvm::makeArrayRef(argBegin, argEnd)), argv0(argv0), main(main) {  }
