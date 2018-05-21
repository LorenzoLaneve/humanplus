// => src/diagnostics/diagnostics.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/diagnostics/diagnostics.h>
#include <hpc/utils/printers.h>
#include <hpc/utils/strings.h>
#include <hpc/utils/opts.h>

#include <map>
#include <string>
#include <sstream>
#include <algorithm>

#define HUMAN_PLUS_ERROR_COUNTER_LIMIT 10

using namespace hpc;

static std::map<diag::DiagID, std::string> diagStrings = {

#define __predef_diag(ID, text) { diag::ID, text },
#include <hpc/inc/diags.inc>
    
#undef __predef_diag
    
};


diag::Diagnostic::Diagnostic(DiagEngine &engine, DiagLevel level, std::string text, src::SrcLoc *srcloc)
: engine(engine), level(level), text(text), srcloc(srcloc) {
    // A little algorithm searching for the %i with the max i. e.g.: if %8 if the max index contained in text, then we asssume the diagnostic needs 9 params.
    int maxIndex = -1;
    size_t pos = 0;
    
    while ((pos = text.find("%", pos)) != std::string::npos) {
        if (++pos < text.length()) {
            int index = text[pos] - '0';
            assert(0 <= index && index <= 9 && "Invalid param placeholder.");
            
            if (index >= maxIndex) {
                maxIndex = index;
            }
            
        }
    }
    
    paramsNeeded = maxIndex + 1;
}

diag::Diagnostic &diag::Diagnostic::addParam(std::string param, bool printIfComplete) {
    if (!isComplete()) {
        params.push_back(param);
        
        if (printIfComplete && isComplete()) {
            engine.sendDiag(*this);
        }
    }
    return *this;
}


void diag::DiagnosticsReport::addDiagnostic(Diagnostic &diagnostic) {
    diags.push_back(&diagnostic);
    
    switch (diagnostic.level) {
        case Error:
            errorsReported++;
            break;
        case Warning:
            warningsReported++;
            break;
        case Note:
            notesReported++;
            break;
        default: break;
    }
}

void diag::DiagnosticsReport::passToEngine(DiagEngine &engine, bool printIfComplete) {
    for (Diagnostic *diag : diags)
        engine.reportDiag(diag);
}


diag::Diagnostic &diag::DiagEngine::reportDiag(DiagLevel level, DiagID ID, src::SrcLoc *srcloc) {
    return reportCustomDiag(level, diagStrings[ID], srcloc);
}

diag::Diagnostic &diag::DiagEngine::reportDiag(Diagnostic *diag) {
    if (diag->isComplete()) {
        sendDiag(*diag);
    }
    
    for (DiagnosticsReport *report : reports) report->addDiagnostic(*diag);
    
    return *diag;
}

diag::Diagnostic &diag::DiagEngine::reportCustomDiag(DiagLevel level, std::string text, src::SrcLoc *srcloc) {
    return reportDiag(new Diagnostic(*this, level, text, srcloc));
}

void diag::DiagEngine::addReport(DiagnosticsReport &report) {
    reports.push_back(&report);
}

void diag::DiagEngine::removeReport(DiagnosticsReport &report) {
    reports.erase(std::remove(reports.begin(), reports.end(), &report), reports.end());
}

void diag::DiagEngine::printReport(DiagnosticsReport &report) {
    unsigned errs = report.errorsReported;
    unsigned warns = report.warningsReported;
    
    if (errs || warns) {
        llvm::outs() << "\n";
        if (errs) {
            if (errs > 1) {
                llvm::outs() << errs << " errors";
            } else {
                llvm::outs() << errs << " error";
            }
            
            if (warns) {
                llvm::outs() << " and ";
            }
        }
        
        if (warns) {
            if (warns > 1) {
                llvm::outs() << warns << " warnings";
            } else {
                llvm::outs() << warns << " warning";
            }
        }
        
        llvm::outs() << " generated.\n";
    }
}

void diag::DiagEngine::printAndRemoveReport(DiagnosticsReport &report) {
    removeReport(report);
    printReport(report);
}

void diag::DiagEngine::sendDiag(Diagnostic &diag) {
    assert(diag.isComplete() && "diag is not complete.");
    
    output.handleDiag(diag);
    
    switch (diag.getLevel()) {
        case Error:
            reportedErrors++;
            break;
        case Warning:
            reportedWarnings++;
            break;
        case Note:
            reportedNotes++;
            break;
        default: break;
    }
}
