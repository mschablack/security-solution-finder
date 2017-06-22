// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file defines the MainAnalysis class. This class represent the
// Analysis done by security-solution-finder.

#ifndef MAINANALYSIS_H
#define MAINANALYSIS_H

#include <vector>
#include "clang/Tooling/Tooling.h"
#include "main-analysis/mainanalysisaction.h"
#include "main-analysis/consumerfactory.h"
#include "main-analysis/visitorfactory.h"
#include "main-analysis/sources.h"
#include "security-patterns/patternanalysis.h"

using namespace std;
using namespace clang;
using namespace clang::tooling;

class MainAnalysis
{
public:
    MainAnalysis(ClangTool tool, shared_ptr<Sources> sources);

    void addPatternAnalysis(shared_ptr<PatternAnalysis> pa);

    int startAnalysis();

    void printResults(raw_ostream &out) const;

    std::unique_ptr<FrontendActionFactory> getFrontendActionFactory();

private:
    vector<shared_ptr<PatternAnalysis> > _analyses;
    ClangTool _tool;
    shared_ptr<Sources> _sources;
};

#endif // MAINANALYSIS_H
