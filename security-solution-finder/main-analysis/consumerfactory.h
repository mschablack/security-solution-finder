// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file defines the ConsumerFactory, which is used to transport the
// ASTConsumer from the PatternAnalysis to the MainConsumer.

#ifndef CONSUMERFACTORY_H
#define CONSUMERFACTORY_H

#include <vector>
#include "clang/AST/ASTConsumer.h"
#include "security-patterns/patternanalysis.h"

using namespace std;

class ConsumerFactory
{
public:
    ConsumerFactory(vector<shared_ptr<PatternAnalysis> > analyses);

    vector<unique_ptr<ASTConsumer> > getASTConsumers(ASTContext *context);

private:
    vector<shared_ptr<PatternAnalysis> > _analyses;
};

#endif // CONSUMERFACTORY_H
