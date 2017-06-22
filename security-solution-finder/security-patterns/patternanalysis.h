// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// The interface each security pattern you want to search for needs to implement.

#ifndef PATTERNANALYSIS_H
#define PATTERNANALYSIS_H

#include <iostream>
#include <vector>
#include "clang/AST/ASTConsumer.h"
#include "clang/Tooling/Tooling.h"
#include "main-analysis/recursivevisitor.h"

using namespace std;
using namespace clang;
using namespace llvm;

class PatternAnalysis
{
public:
    virtual void doPreASTAnalysis() = 0;
    virtual void doPostASTAnalysis() = 0;
    virtual vector<unique_ptr<ASTConsumer> > getASTConsumers(ASTContext *context) = 0;
    virtual vector<unique_ptr<RecursiveVisitor> > getRecursiveVisitors(
            ASTContext *context) = 0;
    virtual void printResults(raw_ostream &out) const = 0;
};

#endif // PATTERNANALYSIS_H
