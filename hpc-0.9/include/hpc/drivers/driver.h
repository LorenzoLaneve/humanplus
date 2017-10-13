//
//  driver.h
//  HumanPlus
//
//  Created by Lore on 22/01/2017.
//  Copyright © 2017 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_driver
#define __human_plus_compiler_driver

#include <llvm/ADT/ArrayRef.h>

#include <string>

namespace hpc {
    namespace drivers {
        
        class Driver {
            llvm::ArrayRef<const char *> args;
            
            const char **argv0;
            
            void *main;
            
        public:
            Driver(const char **argBegin, const char **argEnd, const char **argv0, void *main);
            
            /*!
             \brief Returns the compiler file name.
             */
            std::string getCompilerPath();
            
        };
        
    }
}

#endif
