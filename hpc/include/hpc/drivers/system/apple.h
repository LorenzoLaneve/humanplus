// => hpc/drivers/system/apple.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_system_drivers_apple
#define __human_plus_compiler_system_drivers_apple

#include <hpc/drivers/system/darwin.h>
#include <hpc/utils/opts.h>

#include <sstream>
#include <string>

namespace hpc {
    namespace drivers {
        
        /*!
         \brief Driver for all the Apple systems.
         */
        class AppleDriver : public DarwinDriver {
        protected:
            std::string getXcodeToolchainPath();
            
        public:
            virtual std::string getLinkerPath();
            virtual int invokeLinker(opts::LinkOptions &options);
            
            virtual void addOSLinkOptions(opts::LinkOptions &options, std::ostringstream &flags) = 0;
            
        };
        
        /*!
         \brief Driver for all the MacOS systems.
         */
        class MacOSDriver : public AppleDriver {
        public:
            virtual void addOSLinkOptions(opts::LinkOptions &options, std::ostringstream &flags);
            
        };
        
    }
}


#endif
