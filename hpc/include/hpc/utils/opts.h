// => hpc/utils/opts.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_options
#define __human_plus_compiler_options

#include <hpc/utils/files.h>

#include <llvm/ADT/StringMap.h>
#include <llvm/Option/OptTable.h>

#include <string>
#include <vector>

namespace hpc {
    class Compilation;
    
    namespace opts {

        typedef enum {
            Nothing = 0,
#define __opt(PREFIX, ID, TYPE, GROUP, ALIAS, FLAGS, ALIASARGS, HELPTEXT, METAVAR) ID,
#include <hpc/inc/opts.inc>
#undef __opt
            NumberOfOptions
        } ID;
        
        class OptionsTable : public llvm::opt::OptTable {
            
        public:
            OptionsTable();
        };
        
        /*!
         \brief Returns the options table.
         */
        OptionsTable *getTable();
        
        /*!
         \brief Struct containing all the options passed to the frontend.
         */
        struct FrontendOptions {
            /*!
             \brief A vector containing the files the compiler should compile/take and include to the final program.
             \warning Values on this vector <u>might</u> be duplicated. Filter this vector and don't make the compiler use a file twice, or the linker will give error.
             */
            std::vector<fsys::File *> inputFiles;
            /*!
             \brief The output file, which can be "-" or the pathname (either absolute or relative) for the file to be overwritten.
             */
            std::string outputFile = "";
            /*!
             \brief Returns a \c FileType value indicating the output type.
             */
            fsys::FileType outputType = fsys::Executable;
            /*!
             \brief A boolean indicating whether the user has requested help
             */
            bool showHelp = false;
            /*!
             \brief A boolean indicating whether the user has requested help
             */
            bool showVersion = false;
            /*!
             \brief A boolean indicating whether the user has requested a verbosed output (-v).
             */
            bool verbose = false;
            
            
            ~FrontendOptions();
        };
        
        /*!
         \brief Struct containing all the options about diagnostics reporting.
         */
        struct DiagnosticsOptions {
            /*!
             \brief The name the compiler was called by the command line with.
             */
            std::string compilerName;
        };
        
        /*!
         \brief Struct containing all the options about compilation target.
         */
        struct TargetOptions {
            /*!
             \brief The LLVM triple for the target system.
             */
            std::string targetTriple;
            /*!
             \brief The name of the target system CPU, if specified.
             */
            std::string CPU;
            /*!
             \brief The list of the target features, if specified. These features are specified with a "+" or "-" prefix as specified by LLVM.
             */
            std::vector<std::string> features;
        };
        
        /*!
         \brief Struct containing all the options which will be given to the backend.
         */
        struct BackendOptions {
            /*!
             \brief The optimization level for the codegen (-O).
             */
            unsigned optimizationLevel = 0;
            /*!
             \brief The size level for the codegen (-Os => 1, -Oz => 2).
             */
            unsigned sizeLevel = 0;
        };
        
        /*!
         \brief Defines system-independent options which can be given to the linker.
         */
        struct LinkOptions {
            
            /*!
             \brief List of object/archives the linker has to link.
             */
            std::vector<fsys::File *> inputFiles;
            /*!
             \brief The output file, which can be "-" or the pathname (either absolute or relative) for the file to be overwritten.
             */
            std::string outputFile = "";
            
        };
                
    }
}

#endif
