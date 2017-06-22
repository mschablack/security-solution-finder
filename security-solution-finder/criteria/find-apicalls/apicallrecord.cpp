// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file implements the APICallRecord.

#include "apicallrecord.h"

string APICallRecord::funcName() const
{
    return _funcName;
}

void APICallRecord::setFuncName(const string &funcName)
{
    _funcName = funcName;
}

string APICallRecord::fileName() const
{
    return _fileName;
}

void APICallRecord::setFileName(const string &fileName)
{
    _fileName = fileName;
}

string APICallRecord::apiName() const
{
    return _apiName;
}

void APICallRecord::setApiName(const string &apiName)
{
    _apiName = apiName;
}

unsigned APICallRecord::line() const
{
    return _line;
}

void APICallRecord::setLine(const unsigned &line)
{
    _line = line;
}

unsigned APICallRecord::col() const
{
    return _col;
}

void APICallRecord::setCol(const unsigned &col)
{
    _col = col;
}

bool APICallRecord::less(const APICallRecord &apiRec1, const APICallRecord &apiRec2)
{
    if(apiRec1.apiName().compare(apiRec2.apiName()) < 0) {
        return true;
    } else if (apiRec1.apiName().compare(apiRec2.apiName()) == 0) {
        if(apiRec1.funcName().compare(apiRec2.funcName()) < 0){
            return true;
        } else if(apiRec1.funcName().compare(apiRec2.funcName()) == 0){
            if(apiRec1.fileName().compare(apiRec2.fileName()) < 0){
                return true;
            } else if(apiRec1.fileName().compare(apiRec2.fileName()) == 0){
                if(apiRec1.line() < apiRec2.line()){
                    return true;
                } else if(apiRec1.line() == apiRec2.line() && apiRec1.col() < apiRec2.col()){
                    return true;
                }// End else-if line and column
            }// End else-if fileName
        }// End else-if funcName
    }// End else-if apiName

    return false;
}

void APICallRecord::print() const
{
    print(llvm::outs());
}

void APICallRecord::print(raw_ostream &out) const
{
    out << "Found " << _apiName << "-API-Call: "
        << _funcName << " at\n"
        << _fileName << " " << to_string(_line) << ":" << to_string(_col) << "\n";
}
