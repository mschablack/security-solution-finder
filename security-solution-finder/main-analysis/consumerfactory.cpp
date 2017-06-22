// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file implements the ConsumerFactory.

#include "consumerfactory.h"

ConsumerFactory::ConsumerFactory(vector<shared_ptr<PatternAnalysis> > analyses)
    : _analyses(analyses)
{

}

vector<unique_ptr<ASTConsumer> > ConsumerFactory::getASTConsumers(ASTContext *context)
{
    vector<unique_ptr<ASTConsumer> > result;

    for(shared_ptr<PatternAnalysis> pa : _analyses)
    {
        vector<unique_ptr<ASTConsumer> > pa_astconsumers = pa->getASTConsumers(context);
        result.insert( result.end(),
                       std::make_move_iterator(pa_astconsumers.begin()),
                       std::make_move_iterator(pa_astconsumers.end()) );
    }

    return result;
}
