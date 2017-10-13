//
//  target.hpp
//  HumanPlus
//
//  Created by Lore on 06/04/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_target_descriptor
#define __human_plus_compiler_target_descriptor

#include <hpc/target/abi.h>
#include <hpc/utils/opts.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/ir/modules.h>

#include <llvm/ADT/Triple.h>
#include <llvm/Analysis/TargetLibraryInfo.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/Target/TargetMachine.h>

#include <string>
#include <vector>

namespace hpc {
    
    namespace target {
        
        class TargetInfo {
            /*!
             \brief The target options used to create this target.
             */
            opts::TargetOptions &targetOptions;
            /*!
             \brief The ABI associated to this target.
             */
            TargetABI *theABI;
            
            /*!
             \brief The LLVM triple for the target.
             */
            std::string triple;
            /*!
             \brief The data layout used by this target.
            */
            llvm::DataLayout *datalayout = nullptr;
            
            /*!
             \brief The name of the CPU for this target.
             */
            std::string CPU;
            /*!
             \brief The feature string for this target.
             */
            std::string featureString;
            
            /*!
             \brief The target machine instantiated for this target.
             \note \c createTargetMachine() must be called in order for this to be non-null.
             */
            llvm::TargetMachine *targetMachine = nullptr;
            
            
            TargetInfo(opts::TargetOptions &targetOptions);
            
        public:
            static TargetInfo *fromOptions(opts::TargetOptions &targetOpts, diag::DiagEngine &diags);
            
            /*!
             \brief Returns the target options used to create this target.
             */
            inline opts::TargetOptions &getOptions() const {
                return targetOptions;
            }
            /*!
             \brief Returns the ABI used.
             */
            inline TargetABI &getTargetABI() const {
                assert(theABI && "No ABI used.");
                return *theABI;
            }
            
            /*!
             \brief Returns the LLVM triple for the target.
             */
            std::string getTriple() const {
                return triple;
            }
            
            /*!
             \brief Returns the CPU name for this target.
             */
            std::string getCPUName() const {
                return CPU;
            }
            
            /*!
             \brief Returns a string containing the enabled or disabled features for this target. The string is formatted like <code>+attr1,-attr2,...,+attrN</code>
             */
            std::string getFeatureString() const {
                return featureString;
            }
            
            /*!
             \brief Creates a new ABI object based on the target options.
             */
            bool createTargetABI();
            
            /*!
             \brief Returns the mangler object for the associated ABI.
             */
            MangleSpec &getMangle() const {
                return getTargetABI().getMangle();
            }
            
            /*!
             \brief Creates the TargetLibraryInfo object for the target.
             \returns The object, or \c nullptr if we've been unable to create it.
             */
            llvm::TargetLibraryInfoImpl *createTLII();
            
            /*!
             \brief Creates the target machine.
             \param diags If LLVM could not create target, an error will be reported here. 
             */
            bool createTargetMachine(opts::BackendOptions &backendOptions, diag::DiagEngine &diags);
            
            /*!
             \brief Returns the target machine for this target.
             \note \c createMachineTarget() must be called first, or the function will assert.
             */
            llvm::TargetMachine *getTargetMachine() const {
                assert(targetMachine && "No target machine created.");
                return targetMachine;
            }
            /*!
             \brief Returns the data layout used by this target.
             \note \c createMachineTarget() must be called first, or the function will assert.
             */
            llvm::DataLayout &getDataLayout();
            
            /*!
             \brief Adds the target attributes to the functions of the given module.
             */
            void addTargetAttribs(modules::ModuleWrapper &wrapper);
            
        };
        
    }
}

#endif
