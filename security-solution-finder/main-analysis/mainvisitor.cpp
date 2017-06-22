// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// Implementation of the RecursiveASTVisitor for the main analysis.

#include "mainvisitor.h"

MainVisitor::MainVisitor(ASTContext *Context, VisitorFactory cfactory)
    : _astContext(Context), _visitors(std::move(cfactory.getRecursiveVisitors(Context)))
{

}

bool MainVisitor::VisitCallExpr(CallExpr *callExpr)
{
    bool result = true;
    for (auto &visitor : _visitors)
    {
        result = result && visitor->VisitCallExpr(callExpr);
    }
    return result;
}

bool MainVisitor::VisitClassTemplateDecl(ClassTemplateDecl *Declaration)
{
    bool result = true;
    for (auto &visitor : _visitors)
    {
        result = result && visitor->VisitClassTemplateDecl(Declaration);
    }
    return result;
}

bool MainVisitor::VisitCXXMemberCallExpr(CXXMemberCallExpr *callExpr)
{
    bool result = true;
    for (auto &visitor : _visitors)
    {
        result = result && visitor->VisitCXXMemberCallExpr(callExpr);
    }
    return result;
}

bool MainVisitor::VisitFieldDecl(FieldDecl *Declaration)
{
    bool result = true;
    for (auto &visitor : _visitors)
    {
        result = result && visitor->VisitFieldDecl(Declaration);
    }
    return result;
}

bool MainVisitor::VisitFunctionDecl(FunctionDecl *Declaration)
{
    bool result = true;
    for (auto &visitor : _visitors)
    {
        result = result && visitor->VisitFunctionDecl(Declaration);
    }
    return result;
}

bool MainVisitor::VisitFunctionTemplateDecl(FunctionTemplateDecl *Declaration)
{
    bool result = true;
    for (auto &visitor : _visitors)
    {
        result = result && visitor->VisitFunctionTemplateDecl(Declaration);
    }
    return result;
}

bool MainVisitor::VisitNamespaceDecl(NamespaceDecl *Declaration)
{
    bool result = true;
    for (auto &visitor : _visitors)
    {
        result = result && visitor->VisitNamespaceDecl(Declaration);
    }
    return result;
}

bool MainVisitor::VisitRecordDecl(RecordDecl *Declaration)
{
    bool result = true;
    for (auto &visitor : _visitors)
    {
        result = result && visitor->VisitRecordDecl(Declaration);
    }
    return result;
}

bool MainVisitor::VisitVarDecl(VarDecl *Declaration)
{
    bool result = true;
    for (auto &visitor : _visitors)
    {
        result = result && visitor->VisitVarDecl(Declaration);
    }
    return result;
}

bool MainVisitor::VisitVarTemplateDecl(VarTemplateDecl *Declaration)
{
    bool result = true;
    for (auto &visitor : _visitors)
    {
        result = result && visitor->VisitVarTemplateDecl(Declaration);
    }
    return result;
}
