// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// Abstraction of the RecursiveASTVisitor needed to handle RecursiveASTVisitors
// within vectors (and other compilations)
// Careful: Each Visit*-function you want to use in the specific RecursiveVisitor
// needs to be defined here and hide the RecursiveASTVisitor's original.

#ifndef RECURSIVEVISITOR_H
#define RECURSIVEVISITOR_H

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Tooling/Tooling.h"


using namespace clang;

class RecursiveVisitor : public RecursiveASTVisitor<RecursiveVisitor>
{
public:
    RecursiveVisitor() {}

    virtual bool VisitCallExpr(CallExpr *callExpr) {return true;}
    virtual bool VisitClassTemplateDecl(ClassTemplateDecl *Declaration) {return true;}
    virtual bool VisitCXXMemberCallExpr(CXXMemberCallExpr *callExpr) {return true;}
    virtual bool VisitFieldDecl(FieldDecl *Declaration) {return true;}
    virtual bool VisitFunctionDecl(FunctionDecl *Declaration) {return true;}
    virtual bool VisitFunctionTemplateDecl(FunctionTemplateDecl *Declaration) {return true;}
    virtual bool VisitNamespaceDecl(NamespaceDecl *Declaration) {return true;}
    virtual bool VisitRecordDecl(RecordDecl *Declaration) {return true;}
    virtual bool VisitVarDecl(VarDecl *Declaration) {return true;}
    virtual bool VisitVarTemplateDecl(VarTemplateDecl *Declaration) {return true;}

};

#endif // RECURSIVEVISITOR_H
