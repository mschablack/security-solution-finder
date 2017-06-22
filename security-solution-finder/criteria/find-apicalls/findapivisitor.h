// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file defines the RecurisiveASTVisitor needed for the API-Call criterion.

#ifndef FINDAPIVISITOR_H
#define FINDAPIVISITOR_H

#include <algorithm>
#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclBase.h"
#include "clang/AST/Expr.h"
#include "clang/AST/ExprCXX.h"
#include "main-analysis/recursivevisitor.h"
#include "main-analysis/sources.h"
#include "criteria/find-apicalls/findapicalls.h"
#include "criteria/find-apicalls/apicallrecord.h"


using namespace std;
using namespace clang;
using namespace llvm;


class FindAPIVisitor : public RecursiveVisitor
{
public:
    FindAPIVisitor(ASTContext *context, shared_ptr<FindAPICalls> findAPICalls,
                   shared_ptr<Sources> sources)
        : _astContext(context), _findAPICalls(findAPICalls), _sources(sources) {}

    bool VisitCallExpr(CallExpr *callExpr) override;

private:
    ASTContext *_astContext;
    shared_ptr<FindAPICalls> _findAPICalls;
    shared_ptr<Sources> _sources;
};

#endif // FINDAPIVISITOR_H
