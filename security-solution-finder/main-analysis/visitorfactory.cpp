// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file implements the VisitorFactory.

#include "visitorfactory.h"

VisitorFactory::VisitorFactory(vector<shared_ptr<PatternAnalysis> > analyses)
    : _analyses(analyses)
{

}

vector<unique_ptr<RecursiveVisitor> > VisitorFactory::getRecursiveVisitors(
        ASTContext *context)
{
    vector<unique_ptr<RecursiveVisitor> > result;

    for(shared_ptr<PatternAnalysis> pa : _analyses)
    {
        vector<unique_ptr<RecursiveVisitor> > pa_visitors = pa->getRecursiveVisitors(context);
        result.insert( result.end(),
                       std::make_move_iterator(pa_visitors.begin()),
                       std::make_move_iterator(pa_visitors.end()) );
    }

    return result;
}
