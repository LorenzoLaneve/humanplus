// => src/utils/files.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/utils/files.h>
#include <hpc/target/target.h>
#include <hpc/analyzers/sources.h>

#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/StringSwitch.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/FileUtilities.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/Process.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Signals.h>

#include <iostream>
#include <system_error>

using namespace hpc;

llvm::sys::fs::file_status *fsys::File::getFileStatus(std::error_code *errc) {
    
    llvm::sys::fs::file_status *status = new llvm::sys::fs::file_status();
    
    std::error_code code = llvm::sys::fs::status(fname, *status);
    
    if (errc) {
        *errc = code;
    }
    
    return status;
}

bool fsys::File::exists() {
    return llvm::sys::fs::exists(fname);
}


fsys::InputFile *fsys::InputFile::fromFile(std::string fname) {
    
    FileType fty = llvm::StringSwitch<FileType>(llvm::StringRef(fname).rsplit('.').second)
        .Case("hmn", SourceFile)
        .Case("hmk", HumanPlusKit)
        .Case("ll", TextualIR)
        .Case("bc", BitcodeIR)
        .Default(Unknown);
    
    if (fty == SourceFile) {
        return new src::SourceFile(fname);
    }
    
    return new fsys::InputFile(fname, fty);
}


fsys::OutputFile::OutputFile(std::string fname, llvm::raw_pwrite_stream *stream, FileType ftype, bool shouldKeep)
: File(fname, ftype), stream(stream), shouldKeep(shouldKeep) {
    if (!shouldKeep) {
        llvm::sys::RemoveFileOnSignal(fname);
    }
}

fsys::OutputFile::~OutputFile() {
    delete stream;
    
    if (!shouldKeep) {
        llvm::sys::fs::remove(fname);
    }
    
    llvm::sys::DontRemoveFileOnSignal(fname);
}

void fsys::OutputFile::keep(bool sKeep) {
    shouldKeep = sKeep;

    if (sKeep)
        llvm::sys::DontRemoveFileOnSignal(fname);
    else llvm::sys::RemoveFileOnSignal(fname);

}

fsys::OutputFile *fsys::OutputFile::create(std::string fname, FileType ftype, std::error_code &errcode, bool makeTemp) {
    
    bool isBinary = false;
    std::string outext;
    switch (ftype) {
        case TextualIR: {
            outext = "ll";
            break;
        }
        case BitcodeIR: {
            outext = "bc";
            isBinary = true;
            break;
        }
        case TextualAssembly: {
            outext = "s";
            break;
        }
        case HumanPlusKit: {
            outext = "hmk";
            isBinary = true;
            break;
        }
        case ObjectCode: {
            outext = "o";
            isBinary = true;
            break;
        }
        case Archive: {
            outext = "a";
            isBinary = true;
            break;
        }
        default:
            assert("Invalid output file type.");
    }
    
    llvm::sys::fs::OpenFlags strflags = llvm::sys::fs::F_None;
    if (!isBinary) strflags |= llvm::sys::fs::F_Text;
    
    llvm::raw_fd_ostream *os = nullptr;
    if (makeTemp) {
        llvm::SmallString<128> tmpfile;
        errcode = llvm::sys::fs::createTemporaryFile(llvm::sys::path::stem(fname), outext, tmpfile);
        fname = tmpfile.str();
        
        //llvm::errs() << "\ntmpfile = " << tmpfile << "\n";
        if (errcode)
            return nullptr;
        else {
            os = new llvm::raw_fd_ostream(fname, errcode, strflags);
            
            if (errcode) return nullptr;
        }
    } else {
        llvm::SmallString<128> path(fname);
        llvm::sys::path::replace_extension(path, outext);
        llvm::sys::fs::make_absolute(path);
        fname = path.str();
        
        os = new llvm::raw_fd_ostream(fname, errcode, strflags);
    }
    
    return new OutputFile(fname, os, ftype, !makeTemp);
}

fsys::OutputFile *fsys::OutputFile::createTemp(std::string name, FileType ftype, std::error_code &errcode) {
    return create(name, ftype, errcode, true);
}


