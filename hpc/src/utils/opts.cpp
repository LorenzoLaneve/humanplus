// => src/utils/opts.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/utils/opts.h>
#include <hpc/utils/printers.h>
#include <hpc/utils/strings.h>
#include <hpc/diagnostics/diagnostics.h>

#include <llvm/Option/Option.h>

#define HELP_FLAG_INFO_INDENTATION 26

using namespace hpc;

namespace pref {
#define __opt(PREFIX, ID, TYPE, GROUP, ALIAS, FLAGS, ALIASARGS, HELPTEXT, METAVAR) static const char *const ID[] = { PREFIX, 0 };
#include <hpc/inc/opts.inc>
#undef __opt
}

static const llvm::opt::OptTable::Info options[] = {
#define __opt(PREFIX, ID, TYPE, GROUP, ALIAS, FLAGS, ALIASARGS, HELPTEXT, METAVAR) \
    { pref::ID, "", HELPTEXT, METAVAR, opts::ID, llvm::opt::Option::TYPE##Class, 0, FLAGS, opts::GROUP, opts::ALIAS, ALIASARGS },
#include <hpc/inc/opts.inc>
#undef __opt
};

opts::OptionsTable::OptionsTable() : llvm::opt::OptTable(options) {  }

opts::OptionsTable *opts::getTable() {
    return new OptionsTable();
}

opts::FrontendOptions::~FrontendOptions() {
//    for (fsys::File *file : inputFiles) {
//        delete file;
//    }
}
