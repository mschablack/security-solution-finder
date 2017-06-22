// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file defines the VisitorFactory, which is used to transport the
// RecursiveVisitors (NOT RecursiveASTVisitor) from the PatternAnalysis
// to the Mainvisitor.

#ifndef VISITORFACTORY_H
#define VISITORFACTORY_H

#include <vector>
#include "main-analysis/recursivevisitor.h"
#include "security-patterns/patternanalysis.h"

class VisitorFactory
{
public:
    VisitorFactory(vector<shared_ptr<PatternAnalysis> > analyses);

    vector<unique_ptr<RecursiveVisitor> > getRecursiveVisitors(ASTContext *context);

private:
    vector<shared_ptr<PatternAnalysis> > _analyses;
};

#endif // VISITORFACTORY_H
