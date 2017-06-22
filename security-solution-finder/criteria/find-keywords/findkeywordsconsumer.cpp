// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file implements ASTConsumer for the Find-Keyword criterion.

#include "findkeywordsconsumer.h"


void FindKeywordsConsumer::HandleTranslationUnit(ASTContext &Context)
{
    SourceManager &srcMgr = _astContext->getSourceManager();
    RawCommentList comments = _astContext->getRawCommentList();
    ArrayRef<RawComment*> comments_arr = comments.getComments();
    for (RawComment* c: comments_arr){
        StringRef rt = c->getRawText(srcMgr);
        for(string keyword : _findKeywords->getKeywords()){
            bool contains = rt.contains_lower(keyword);
            if (contains) {
                FullSourceLoc FullLocation = _astContext->getFullLoc(c->getLocStart());
                if (FullLocation.isValid())
                {
                    FoundKeywordRecord fkr (keyword, srcMgr.getFilename(FullLocation),
                                            FullLocation.getSpellingLineNumber(),
                                            FullLocation.getSpellingColumnNumber(),
                                            FoundKeywordType::COMMENT);
                    fkr.print();
                    _findKeywords->addFoundKeyword(fkr);
                }
            }
        } //END for - keywords
    } //END for - comments
}
