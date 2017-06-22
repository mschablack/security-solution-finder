// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.

#include <chrono>
#include <iostream>
#include <fstream>
#include "clang/Driver/Options.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "main-analysis/mainanalysis.h"
#include "main-analysis/sources.h"
#include "security-patterns/patternanalysis.h"
#include "security-patterns/encryptedstorageanalysis.h"
#include "security-patterns/sandboxanalysis.h"

using namespace std;
using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;

static llvm::cl::OptionCategory MyToolCategory("security-solution-finder options");
static cl::opt<std::string> OutPutFile("ofile", cl::cat(MyToolCategory));

int main(int argc, const char **argv) {
    auto startTime = std::chrono::high_resolution_clock::now();

    CommonOptionsParser op(argc, argv, MyToolCategory);

    shared_ptr<Sources> sources(new Sources(op.getSourcePathList()));

    ClangTool Tool(op.getCompilations(), op.getSourcePathList());
    Tool.appendArgumentsAdjuster(
                tooling::getInsertArgumentAdjuster(
                    "-fparse-all-comments", tooling::ArgumentInsertPosition::BEGIN));


    MainAnalysis mainAnalysis (Tool, sources);
    mainAnalysis.addPatternAnalysis(
                shared_ptr<PatternAnalysis>(new SandboxAnalysis(sources)));
    mainAnalysis.addPatternAnalysis(
                shared_ptr<PatternAnalysis>(new EncryptedStorageAnalysis(sources)));

    int result = mainAnalysis.startAnalysis();

    StringRef oFilePath = OutPutFile.getValue();
    if(!oFilePath.empty())
    {
        std::error_code EC = std::error_code();
        raw_fd_ostream oFile(oFilePath, EC, sys::fs::F_None);
        mainAnalysis.printResults(oFile);
        oFile.close();
    }

    auto duration = std::chrono::duration_cast<std::chrono::seconds>
            (std::chrono::high_resolution_clock::now() - startTime);

    llvm::outs() << "Time used for Analyzing: "
                 << duration.count() << " Seconds.\n";

    return result;
}
