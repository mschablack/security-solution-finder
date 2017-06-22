// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file defines the SandboxAnalysis. This Class holds all information
// needed to search for the Sandbox Pattern.

#ifndef SANDBOXANALYSIS_H
#define SANDBOXANALYSIS_H

#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include "clang/AST/ASTConsumer.h"
#include "clang/Tooling/Tooling.h"
#include "main-analysis/recursivevisitor.h"
#include "main-analysis/sources.h"
#include "criteria/find-keywords/findkeywords.h"
#include "criteria/find-keywords/findkeywordsconsumer.h"
#include "criteria/find-keywords/findkeywordsvisitor.h"
#include "criteria/find-apicalls/findapicalls.h"
#include "criteria/find-apicalls/findapivisitor.h"
#include "security-patterns/patternanalysis.h"

using namespace std;
using namespace clang;
using namespace llvm;

class SandboxAnalysis : public PatternAnalysis
{
public:
    SandboxAnalysis(shared_ptr<Sources> sources);
    void doPreASTAnalysis() override;
    void doPostASTAnalysis() override;
    vector<unique_ptr<ASTConsumer> > getASTConsumers(ASTContext *context) override;
    vector<unique_ptr<RecursiveVisitor> > getRecursiveVisitors(ASTContext *context) override;
    void printResults(raw_ostream &out) const override;

private:
    shared_ptr<FindKeywords> _findKeywords;
    shared_ptr<FindAPICalls> _findAPICalls;
    shared_ptr<Sources> _sources;
    multimap<double,string> _fileRatings;
    double _threshold;

    void prepareKeywords();
    void prepareAPICalls();
    string double_to_string(double d) const;
};

#endif // SANDBOXANALYSIS_H
