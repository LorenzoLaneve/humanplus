// => src/drivers/system/unix.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/drivers/system/unix.h>
#include <hpc/utils/printers.h>
#include <hpc/utils/strings.h>

#include <llvm/ADT/Triple.h>
#include <llvm/Support/Host.h>

#include <sstream>

using namespace hpc;

std::string drivers::UnixDriver::getLinkerPath() {
    return "/usr/bin/ld";
}

int drivers::UnixDriver::invokeLinker(opts::LinkOptions &options) {
    // Call system linker
    std::ostringstream flags;
    flags << getLinkerPath();
    
    flags << " /usr/lib/" << llvm::Triple::getArchTypeName(llvm::Triple(llvm::sys::getDefaultTargetTriple()).getArch()).str() << "-gnu-linux/crt1.o";
    flags << " /usr/lib/" << llvm::Triple::getArchTypeName(llvm::Triple(llvm::sys::getDefaultTargetTriple()).getArch()).str() << "-gnu-linux/crti.o";
    
    flags << " -lc";
    flags << " -lhumanlogic";
    
    for (fsys::File *file : options.inputFiles) {
        flags << " " << util::bashEncode(file->getFileName());
    }
    
    flags << " /usr/lib/" << llvm::Triple::getArchTypeName(llvm::Triple(llvm::sys::getDefaultTargetTriple()).getArch()).str() << "-gnu-linux/crtn.o";
    
    if (!options.outputFile.empty())
        flags << " -o " << util::bashEncode(options.outputFile);
    
    // FIXME this should be passed to linux drivers
    
    
    int linkexit = std::system(flags.str().c_str());
    
    return WEXITSTATUS(linkexit);
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

