// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// The RecursiveASTVisitor for the main analysis.
// This class acts similar to the MultiplexConsumer, but for RecursiveASTVisitors.

#ifndef MAINVISITOR_H
#define MAINVISITOR_H


#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclBase.h"
#include "main-analysis/recursivevisitor.h"
#include "main-analysis/visitorfactory.h"

using namespace std;
using namespace clang;
using namespace llvm;


class MainVisitor : public RecursiveASTVisitor<MainVisitor>
{
public:
    explicit MainVisitor(ASTContext *Context, VisitorFactory cfactory);
    bool VisitCallExpr(CallExpr *callExpr);
    bool VisitClassTemplateDecl(ClassTemplateDecl *Declaration);
    bool VisitCXXMemberCallExpr(CXXMemberCallExpr *callExpr);
    bool VisitFieldDecl(FieldDecl *Declaration);
    bool VisitFunctionDecl(FunctionDecl *Declaration);
    bool VisitFunctionTemplateDecl(FunctionTemplateDecl *Declaration);
    bool VisitNamespaceDecl(NamespaceDecl *Declaration);
    bool VisitRecordDecl(RecordDecl *Declaration);
    bool VisitVarDecl(VarDecl *Declaration);
    bool VisitVarTemplateDecl(VarTemplateDecl *Declaration);

private:
    ASTContext *_astContext;
    vector<unique_ptr<RecursiveVisitor> > _visitors;
};

#endif // MAINVISITOR_H
