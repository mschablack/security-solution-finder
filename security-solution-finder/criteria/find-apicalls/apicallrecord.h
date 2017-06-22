// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file defines the APICallRecord class. This class is used to store
// a single API-call found during the analysis.

#ifndef APICALLRECORD_H
#define APICALLRECORD_H

#include "clang/Tooling/Tooling.h"


using namespace std;
using namespace clang;
using namespace llvm;

class APICallRecord
{
public:
    APICallRecord(string funcName, string apiName, string fileName,
                  unsigned line, unsigned col)
        : _funcName(funcName), _apiName(apiName), _fileName(fileName),
          _line(line), _col(col) {}

    string funcName() const;
    void setFuncName(const string &funcName);

    string fileName() const;
    void setFileName(const string &fileName);

    string apiName() const;
    void setApiName(const string &apiName);

    unsigned line() const;
    void setLine(const unsigned &line);

    unsigned col() const;
    void setCol(const unsigned &col);

    static bool less(const APICallRecord &apiRec1, const APICallRecord &apiRec2);

    void print() const;
    void print(raw_ostream &out) const;

private:
    string _funcName;
    string _apiName;
    string _fileName;
    unsigned _line;
    unsigned _col;
};

#endif // APICALLRECORD_H
