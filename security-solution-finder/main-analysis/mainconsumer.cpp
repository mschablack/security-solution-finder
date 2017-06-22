// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// The file implements the MainConsumer class.

#include "mainconsumer.h"

MainConsumer::MainConsumer(ASTContext *Context,
                           vector<unique_ptr<ASTConsumer> > consumers,
                           VisitorFactory vfactory)
    : MultiplexConsumer(std::move(consumers)),
      _visitor(Context, vfactory), _astContext(Context)
{

}

void MainConsumer::HandleTranslationUnit(ASTContext &Context)
{
    MultiplexConsumer::HandleTranslationUnit(Context);

    _visitor.TraverseDecl(Context.getTranslationUnitDecl());
}
