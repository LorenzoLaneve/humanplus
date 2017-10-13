//
//  unix.h
//  HumanPlus
//
//  Created by Lore on 20/07/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_system_drivers_unix
#define __human_plus_compiler_system_drivers_unix

#include <hpc/drivers/system/system.h>
#include <hpc/utils/opts.h>

namespace hpc {
    namespace drivers {
        
        /*!
         \brief Subclass of \c SystemDriver which implements the driver for any Unix-like/Unix-based platforms.
         */
        class UnixDriver : public SystemDriver {
            
        public:
            virtual ~UnixDriver() {  }
            
            virtual std::string getLinkerPath();
            virtual int invokeLinker(opts::LinkOptions &options);
            virtual fsys::FileType typeForExtension(std::string fext);
            virtual std::string extensionForType(fsys::FileType ftype);
            virtual std::string defaultAssemblerOutputName();
            
        };
        
    }
}

#endif
