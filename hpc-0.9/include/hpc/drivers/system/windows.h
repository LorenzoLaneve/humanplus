//
//  windows.h
//  HumanPlus
//
//  Created by Lore on 04/11/2016.
//  Copyright © 2016 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_system_drivers_windows
#define __human_plus_compiler_system_drivers_windows

#include <hpc/drivers/system/system.h>
#include <hpc/utils/opts.h>

namespace hpc {
    namespace drivers {
        
        /*!
         \brief Driver for all the Windows systems.
         */
        class WindowsDriver : public SystemDriver {
        public:
            virtual std::string getLinkerPath();
            virtual int invokeLinker(opts::LinkOptions &options);
            virtual std::string defaultAssemblerOutputName();
            
        };

        
    }
}

#endif
