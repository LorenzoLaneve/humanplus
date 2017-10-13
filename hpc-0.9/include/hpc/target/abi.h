//
//  abi.h
//  HumanPlus
//
//  Created by Lore on 15/04/2017.
//  Copyright © 2017 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_target_abi
#define __human_plus_compiler_target_abi

#include <hpc/target/mangle.h>
#include <hpc/utils/opts.h>

#include <string>

namespace hpc {
    
    namespace target {
        
        /*!
         \brief Class which defines an abstraction layer for Application Binary Interfaces used by the Human Plus Compiler.
         */
        class TargetABI {
            
        public:
            /*!
             \brief Values indicating the different ABIs supported by the Human Plus Compiler.
             */
            typedef enum {
                /*!
                 \brief Base ABI for all the C++ Itanium-based ABIs.
                 \brief https://itanium-cxx-abi.github.io/cxx-abi/ for a complete documentation of the ABI.
                 */
                CXXGenericItanium,
                /*!
                 \brief Base ABI for all the C++ ARM-based ABIs. It is derived from the C++ Itanium ABI.
                 \see http://infocenter.arm.com/help/topic/com.arm.doc.ihi0041c/IHI0041C_cppabi.pdf for a complete documentation of the ABI.
                 */
                CXXGenericARM,
                /*!
                 \brief The iOS ABI. It is derived from the C++ ARM ABI.
                 */
                CXXiOS,
                /*!
                 \brief The 64-bit iOS ABI. It is derived from the iOS ABI and offers a closer ABI to the ARM64 ABI.
                 \see http://infocenter.arm.com/help/topic/com.arm.doc.ihi0059a/IHI0059A_cppabi64.pdf for a complete documentation of the ARM64 ABI.
                 */
                CXXiOS64,
                /*!
                 \brief The Watch OS ABI. It's a new version of the 64-bit based iOS ABI ported to 32-bit archs.
                 */
                CXXWatchOS,
                /*!
                 \brief Base ABI for all the C++ AArch64-base ABIs. It is derived from the C++ Itanium ABI but has a few differences with the ARM ABI.
                 */
                CXXGenericAArch64,
                /*!
                 \brief Base ABI for MIPS archs. It is derived from the C++ Itanium ABI.
                 */
                CXXGenericMIPS,
                /*!
                 \brief The Web Assembly ABI. It is derived from the C++ Itanium ABI.
                 */
                CXXWebAssembly,
                /*!
                 \brief The ABI used by MSVC (and compatible compilers) for 32-bit archs.
                 */
                CXXMicrosoft32,
                /*!
                 \brief The ABI used by MSVC (and compatible compilers) for 64-bit archs.
                 */
                CXXMicrosoft64
            } Kind;
            
        private:
            /*!
             \brief The target options used to create this target ABI.
             */
            opts::TargetOptions &opts;
            /*!
             \brief The kind of the ABI.
             */
            Kind kind;
            /*!
             \brief The mangle specification associated to this ABI.
             */
            MangleSpec *mangleSpec;
            
        protected:
            TargetABI(opts::TargetOptions &opts, Kind kind) : opts(opts), kind(kind), mangleSpec(new NeuronMangle()) {  } // FIXME mangle
            
        public:
            virtual ~TargetABI() {  }
            
            /*!
             \brief Returns the kind of the ABI.
             */
            inline Kind getKind() const { return kind; }
            /*!
             \brief Returns the mangle specification object associated to this ABI.
             */
            inline MangleSpec &getMangle() const {
                return *mangleSpec;
            }
            
            /*!
             \brief Returns whether this ABI is a C++ ABI.
             */
            bool isCXXABI() const;
            /*!
             \brief Returns whether this ABI is based on the Itanium ABI.
             */
            bool isItaniumABI() const;
            /*!
             \brief Returns whether this ABI is based on the Microsoft/MSVC ABI.
             */
            bool isMicrosoftABI() const;
            
            
            inline friend bool operator==(const TargetABI &left, const TargetABI &right) {
                return left.getKind() == right.getKind();
            }
            
            inline friend bool operator!=(const TargetABI &left, const TargetABI &right) {
                return !(left == right);
            }
            
            /*!
             \brief Creates an ABI with the given target options.
             */
            static TargetABI *createABI(opts::TargetOptions &opts);
            
        };
        
    }
}

#endif
