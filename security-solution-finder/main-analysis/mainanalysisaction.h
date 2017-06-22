// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// The ASTFrontendAction for the analysis.

#ifndef MAINANALYSISACTION_H
#define MAINANALYSISACTION_H

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "main-analysis/mainconsumer.h"
#include "main-analysis/consumerfactory.h"
#include "main-analysis/visitorfactory.h"
#include "main-analysis/includecapturer.h"
#include "main-analysis/sources.h"


class MainAnalysisAction : public clang::ASTFrontendAction
{
public:
    MainAnalysisAction(ConsumerFactory cfactory,
                       VisitorFactory vfactory,
                       shared_ptr<Sources> sources);
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
            clang::CompilerInstance &compiler, llvm::StringRef InFile);

private:
    ConsumerFactory _cfactory;
    VisitorFactory _vfactory;
    shared_ptr<Sources> _sources;
};

#endif // MAINANALYSISACTION_H
