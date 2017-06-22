// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file implements RecursiveASTVisitor for the Find-Keyword criterion.

#include "findkeywordsvisitor.h"


bool FindKeywordsVisitor::VisitClassTemplateDecl(ClassTemplateDecl *Declaration)
{
    handleNamedDecls(Declaration, FoundKeywordType::CLASSTEMPLATE);
    return true;
}

bool FindKeywordsVisitor::VisitFieldDecl(FieldDecl *Declaration)
{
    handleNamedDecls(Declaration, FoundKeywordType::FIELD);
    return true;
}

bool FindKeywordsVisitor::VisitNamespaceDecl(NamespaceDecl *Declaration)
{
    handleNamedDecls(Declaration, FoundKeywordType::NAMESPACE);
    return true;
}

bool FindKeywordsVisitor::VisitFunctionDecl(FunctionDecl *Declaration)
{
    handleNamedDecls(Declaration, FoundKeywordType::FUNCTION);
    return true;
}

bool FindKeywordsVisitor::VisitFunctionTemplateDecl(FunctionTemplateDecl *Declaration)
{
    handleNamedDecls(Declaration, FoundKeywordType::FUNCTIONTEMPLATE);
    return true;
}

bool FindKeywordsVisitor::VisitRecordDecl(RecordDecl *Declaration)
{
    handleNamedDecls(Declaration, FoundKeywordType::CLASS);
    return true;
}

bool FindKeywordsVisitor::VisitVarDecl(VarDecl *Declaration)
{
    handleNamedDecls(Declaration, FoundKeywordType::VARIABLE);
    return true;
}

bool FindKeywordsVisitor::VisitVarTemplateDecl(VarTemplateDecl *Declaration)
{
    handleNamedDecls(Declaration, FoundKeywordType::VARTEMPLATE);
    return true;
}

void FindKeywordsVisitor::handleNamedDecls(NamedDecl *Declaration, FoundKeywordType type)
{
    SourceManager &srcMgr = _astContext->getSourceManager();
    FullSourceLoc fullLocation = _astContext->getFullLoc(Declaration->getLocStart());
    if (fullLocation.isValid() && !srcMgr.isInSystemHeader(fullLocation)) {
        StringRef name = Declaration->getNameAsString();
        for(string keyword : _findKeywords->getKeywords()){
            bool contains = name.contains_lower(keyword);
            if (contains) {
                string fileName = srcMgr.getFilename(fullLocation).str();
                if(!fileName.empty()) {
                    FoundKeywordRecord fkr (keyword, fileName,
                                            fullLocation.getSpellingLineNumber(),
                                            fullLocation.getSpellingColumnNumber(), type);
                    fkr.print();
                    _findKeywords->addFoundKeyword(fkr);
                }
            }
        } //END for - keywords
    }
}
