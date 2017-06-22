// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// Implementation of the MainAnalysisAction class.

#include "mainanalysisaction.h"

MainAnalysisAction::MainAnalysisAction(ConsumerFactory cfactory,
                                       VisitorFactory vfactory,
                                       shared_ptr<Sources> sources)
    : _cfactory(cfactory), _vfactory(vfactory), _sources(sources)
{

}

std::unique_ptr<clang::ASTConsumer> MainAnalysisAction::CreateASTConsumer(
        clang::CompilerInstance &compiler, llvm::StringRef InFile)
{
    llvm::outs() << "Processing File:\n " << InFile << "\n";
    Preprocessor &pp = compiler.getPreprocessor();
    pp.addPPCallbacks(std::move(
                          unique_ptr<PPCallbacks>(new IncludeCapturer(pp,_sources))));
    ASTContext* context = &compiler.getASTContext();
    return std::unique_ptr<clang::ASTConsumer>(
                new MainConsumer(context,
                                 _cfactory.getASTConsumers(context),
                                 _vfactory));
}
