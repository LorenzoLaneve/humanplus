// => src/diagnostics/output.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/diagnostics/output.h>
#include <hpc/diagnostics/diagnostics.h>
#include <hpc/utils/strings.h>

using namespace hpc;

diag::DiagPrinter::DiagPrinter(llvm::raw_ostream &stream, opts::DiagnosticsOptions &opts) : stream(stream), opts(opts) {  }

void diag::DiagPrinter::handleDiag(Diagnostic &diag) {
    assert(diag.isComplete() && "diag is not complete.");
    
    if (diag.getSrcLoc()) {
        diag.getSrcLoc()->dump();
        stream << ": ";
    } else if (diag.getLevel() != Blank) {
        stream << "hpc: ";
    }
    
    switch (diag.getLevel()) {
        case Error:
            if (stream.has_colors()) stream.changeColor(llvm::buffer_ostream::RED, true);
            stream << "error: ";
            break;
        case Warning:
            if (stream.has_colors()) stream.changeColor(llvm::buffer_ostream::MAGENTA, true);
            stream << "warning: ";
            break;
        case Note:
            if (stream.has_colors()) stream.changeColor(llvm::buffer_ostream::BLACK, true);
            stream << "note: ";
            break;
        default: break;
    }
    if (stream.has_colors()) stream.resetColor();
    
    stream << util::replaceParams(diag.getText(), diag.getParams()) << "\n";
}



diag::DiagBufferer::~DiagBufferer() {
    for (Diagnostic *diag : buffer) {
        delete diag;
    }
}

void diag::DiagBufferer::handleDiag(Diagnostic &diag) {
    buffer.push_back(&diag);
}

void diag::DiagBufferer::passToEngine(DiagEngine &engine) {
    for (Diagnostic *diag : buffer) {
        engine.reportDiag(diag);
        delete diag;
    }
    
    buffer.clear();
}
