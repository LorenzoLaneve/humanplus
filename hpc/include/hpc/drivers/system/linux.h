// => hpc/drivers/system/linux.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
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
