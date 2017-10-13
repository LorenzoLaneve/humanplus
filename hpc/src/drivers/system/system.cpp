// => src/drivers/system/system.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/drivers/system/system.h>
#include <hpc/drivers/system/apple.h>

#include <llvm/ADT/StringSwitch.h>

using namespace hpc;

static drivers::SystemDriver *hostsys = nullptr;

drivers::SystemDriver *drivers::getHostSystem() {
    if (!hostsys) {
        hostsys = new MacOSDriver();
    }
    return hostsys;
}

fsys::FileType drivers::SystemDriver::typeForExtension(std::string fext) {
    // In this implementation there are platform-independent file types, call this method from overrides, on default clauses.
    return llvm::StringSwitch<fsys::FileType>(fext)
                .Case("hmn", fsys::SourceFile)
                .Case("ll",  fsys::TextualIR)
                .Case("bc",  fsys::BitcodeIR)
                .Case("hmk", fsys::HumanPlusKit)
                .Default(fsys::Unknown);
}

std::string drivers::SystemDriver::extensionForType(fsys::FileType ftype) {
    // In this implementation there are platform-independent file types, call this method from overrides, on default clauses.
    switch (ftype) {
        case fsys::SourceFile:
            return "hmn";
        case fsys::TextualIR:
            return "ll";
        case fsys::BitcodeIR:
            return "bc";
        case fsys::HumanPlusKit:
            return "hmk";
        default:
            return "";
    }
}
