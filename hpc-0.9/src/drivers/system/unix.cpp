//
//  unix.cpp
//  HumanPlus
//
//  Created by Lore on 20/07/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <hpc/drivers/system/unix.h>

#include <llvm/ADT/StringSwitch.h>

using namespace hpc;

std::string drivers::UnixDriver::getLinkerPath() {
    return "/usr/bin/ld";
}

int drivers::UnixDriver::invokeLinker(opts::LinkOptions &options) {
    // Call system linker
    return 1;
}

fsys::FileType drivers::UnixDriver::typeForExtension(std::string fext) {
    return llvm::StringSwitch<fsys::FileType>(fext)
                .Case("s",   fsys::TextualAssembly)
                .Case("o",   fsys::ObjectCode)
                .Case("a",   fsys::Archive)
                .Case("",    fsys::Executable)
                .Default(fsys::Unknown);
}

std::string drivers::UnixDriver::extensionForType(fsys::FileType ftype) {
    switch (ftype) {
        case fsys::TextualAssembly:
            return "s";
        case fsys::ObjectCode:
            return "o";
        case fsys::Archive:
            return "a";
        case fsys::Executable:
            return ""; // On unix-like systems executables do not have an extension
        default:
            return SystemDriver::extensionForType(ftype);
    }
}

std::string drivers::UnixDriver::defaultAssemblerOutputName() {
    return "a.out";
}

