// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file defines the ASTConsumer needed for the Find-Keyword criterion.

#ifndef FINDKEYWORDSCONSUMER_H
#define FINDKEYWORDSCONSUMER_H

#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/ASTConsumers.h"
#include "criteria/find-keywords/findkeywords.h"
#include "criteria/find-keywords/foundkeywordrecord.h"


using namespace std;
using namespace clang;
using namespace llvm;


class FindKeywordsConsumer : public ASTConsumer
{
public:
    explicit FindKeywordsConsumer(ASTContext *Context, shared_ptr<FindKeywords> findKeywords)
        : _astContext(Context), _findKeywords(findKeywords) {}
    void HandleTranslationUnit(ASTContext &Context);

private:
    ASTContext *_astContext;
    shared_ptr<FindKeywords> _findKeywords;
};

#endif // FINDKEYWORDSCONSUMER_H
