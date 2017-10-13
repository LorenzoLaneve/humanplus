//
//  linux.h
//  HumanPlus
//
//  Created by Lore on 20/10/2016.
//  Copyright © 2016 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_system_drivers_linux
#define __human_plus_compiler_system_drivers_linux

#include <hpc/drivers/system/unix.h>

namespace hpc {
    namespace drivers {
        
        /*!
         \brief Subclass of \c SystemDriver which implements the driver for any Linux-based platforms.
         */
        class LinuxDriver : public UnixDriver {

        };
        
    }
}

#endif
