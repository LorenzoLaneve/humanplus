// => hpc/drivers/system/system.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_system_drivers
#define __human_plus_compiler_system_drivers

#include <hpc/utils/files.h>
#include <hpc/utils/opts.h>

#include <string>

namespace hpc {
    namespace drivers {
        
        /*!
         \brief Object which provides a standard interface for the system tools the compiler needs to use.
         */
        class SystemDriver {
            
        public:
            virtual ~SystemDriver() {  }
            
            /*!
             \brief Returns the system linker pathname .
             */
            virtual std::string getLinkerPath() = 0;
            /*!
             \brief Invokes the system linker passing the given \c LinkOptions .
             */
            virtual int invokeLinker(opts::LinkOptions &options) = 0;
            /*!
             \brief Returns the file type for the given extension. If the extension is not recognized by the compiler, \c Unknown should be returned.
             \param fext A \c std::string containing the extension (without dot, e.g.: "hmn", "o")
             */
            virtual fsys::FileType typeForExtension(std::string fext);
            /*!
             \brief Returns the file extension for the given file type, without dot. If more than one extension exist for given type, the most popular extension should be returned, for \c Unknown and \c Nothing types, an empty string should be returned.
             */
            virtual std::string extensionForType(fsys::FileType ftype);
            /*!
             \brief Returns the default assembler output name.
             */
            virtual std::string defaultAssemblerOutputName() = 0;
            
        };
        
        /*!
         \brief Returns the driver for the host system.
         */
        SystemDriver *getHostSystem();
        
    }
}

#endif 
