// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file defines the RecurisiveASTVisitor needed for the Find-Keyword criterion.

#ifndef FINDKEYWORDSVISITOR_H
#define FINDKEYWORDSVISITOR_H

#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclBase.h"
#include "clang/AST/DeclTemplate.h"
#include "main-analysis/recursivevisitor.h"
#include "findkeywords.h"
#include "foundkeywordrecord.h"


using namespace std;
using namespace clang;
using namespace llvm;

class FindKeywordsVisitor : public RecursiveVisitor
{
public:
    explicit FindKeywordsVisitor(ASTContext *Context, shared_ptr<FindKeywords> findKeywords)
        : _astContext(Context), _findKeywords(findKeywords) {}
    bool VisitClassTemplateDecl(ClassTemplateDecl *Declaration) override;
    bool VisitFieldDecl(FieldDecl *Declaration) override;
    bool VisitFunctionDecl(FunctionDecl *Declaration) override;
    bool VisitFunctionTemplateDecl(FunctionTemplateDecl *Declaration) override;
    bool VisitNamespaceDecl(NamespaceDecl *Declaration) override;
    bool VisitRecordDecl(RecordDecl *Declaration) override;
    bool VisitVarDecl(VarDecl *Declaration) override;
    bool VisitVarTemplateDecl(VarTemplateDecl *Declaration) override;


private:
    ASTContext *_astContext;
    shared_ptr<FindKeywords> _findKeywords;

    void handleNamedDecls(NamedDecl *Declaration, FoundKeywordType type);
};

#endif // FINDKEYWORDSVISITOR_H
