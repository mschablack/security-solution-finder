// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file defines the FoundKeywordRecord class. This class is used to store
// a single occurrence of a Keyword found during the analysis.
// This class is also responsible for computing the relevance of the
// represented occurrence.

#ifndef FINDKEYWORDSRECORD_H
#define FINDKEYWORDSRECORD_H

#include "clang/Tooling/Tooling.h"


using namespace std;
using namespace clang;
using namespace llvm;

enum FoundKeywordType
{
    CLASS, CLASSTEMPLATE, COMMENT, FIELD, FILENAME, FUNCTION, FUNCTIONTEMPLATE,
    NAMESPACE, VARIABLE, VARTEMPLATE
};

class FoundKeywordRecord
{
public:
    FoundKeywordRecord(string keyword, string fileName, unsigned line,
                       unsigned col, FoundKeywordType type)
        : _keyword(keyword), _fileName(fileName), _line(line), _col(col), _type(type) {}

    FoundKeywordRecord(string keyword, string fileName, FoundKeywordType type)
        : FoundKeywordRecord(keyword, fileName, 0, 0, type) {}

    FoundKeywordType type() const;
    string typeAsString() const;
    void setType(const FoundKeywordType &type);

    string keyword() const;
    void setKeyword(const string &keyword);

    void print() const;
    void print(raw_ostream &out) const;

    string fileName() const;
    void setFileName(const string fileName);

    unsigned line() const;
    void setLine(const unsigned &line);

    unsigned col() const;
    void setCol(const unsigned &col);

    static bool less(const FoundKeywordRecord &fkr1, const FoundKeywordRecord &fkr2);

private:
    string _keyword;
    string _fileName;
    unsigned _line;
    unsigned _col;
    FoundKeywordType _type;
};

#endif // FINDKEYWORDSRECORD_H
