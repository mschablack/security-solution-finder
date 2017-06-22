// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// The ASTConsumer for the analysis.

#ifndef MAINCONSUMER_H
#define MAINCONSUMER_H

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/MultiplexConsumer.h"
#include "main-analysis/mainvisitor.h"
#include "main-analysis/visitorfactory.h"

using namespace clang;

class MainConsumer : public MultiplexConsumer
{
public:
    explicit MainConsumer(ASTContext *Context,
                          vector<unique_ptr<ASTConsumer> > consumers,
                          VisitorFactory vfactory);
    void HandleTranslationUnit(ASTContext &Context);

private:
    MainVisitor _visitor;
    ASTContext *_astContext;
};

#endif // MAINCONSUMER_H
