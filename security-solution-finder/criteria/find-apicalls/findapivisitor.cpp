// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file implements RecursiveASTVisitor for the API-Call criterion.

#include "findapivisitor.h"


bool FindAPIVisitor::VisitCallExpr(CallExpr *callExpr)
{
    SourceManager &srcMgr = _astContext->getSourceManager();
    SourceLocation sloc = callExpr->getLocStart();
    Decl* callee = callExpr->getCalleeDecl();
    if(callee && !srcMgr.isInSystemHeader(sloc)){
        if (NamedDecl *namedCallee = dyn_cast<NamedDecl>(callee))
        {
            string calleeName = namedCallee->getNameAsString();

            FullSourceLoc fullLocation = _astContext->getFullLoc(callExpr->getLocStart());

            if (fullLocation.isValid())
            {

                for(string apiFunc : _findAPICalls->getAPIFunctions())
                {
                    if (calleeName == apiFunc)
                    {
                        string fileName;
                        fileName = srcMgr.getFilename(fullLocation).str();
                        if(!fileName.empty())
                        {
                            string apiName = _findAPICalls->getAPIforFunc(apiFunc);

                            bool apiHeaderIncluded = false;

                            for(StringRef includedHeader :
                                _sources->getAllAvailableIncludesForFile(fileName))
                            {
                                for(StringRef apiHeader :
                                    _findAPICalls->getAPIHeaders(apiName))
                                {
                                    apiHeaderIncluded = apiHeaderIncluded ||
                                            apiHeader.contains_lower(includedHeader)
                                            || includedHeader.contains_lower(apiHeader);
                                }
                            }

                            if(apiHeaderIncluded)
                            {
                                APICallRecord record (apiFunc, apiName, fileName,
                                                      fullLocation.getSpellingLineNumber(),
                                                      fullLocation.getSpellingColumnNumber());

                                record.print();
                                _findAPICalls->addAPICallRecord(record);
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}
