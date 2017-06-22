// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file implements the FoundKeywordRecord class.

#include "foundkeywordrecord.h"

FoundKeywordType FoundKeywordRecord::type() const
{
    return _type;
}

string FoundKeywordRecord::typeAsString() const
{
    string s_type;
    switch(_type)
    {
    case CLASS: s_type = "Class"; break;
    case CLASSTEMPLATE: s_type = "Class-Template"; break;
    case COMMENT: s_type = "Comment"; break;
    case FIELD: s_type = "Field"; break;
    case FUNCTION: s_type = "Function"; break;
    case FUNCTIONTEMPLATE: s_type = "Function-Template"; break;
    case FILENAME: s_type = "Filename"; break;
    case NAMESPACE: s_type = "Namespace"; break;
    case VARIABLE: s_type = "Variable"; break;
    case VARTEMPLATE: s_type = "Variable-Template"; break;
    }
    return s_type;
}

void FoundKeywordRecord::setType(const FoundKeywordType &type)
{
    _type = type;
}

string FoundKeywordRecord::keyword() const
{
    return _keyword;
}

void FoundKeywordRecord::setKeyword(const string &keyword)
{
    _keyword = keyword;
}

void FoundKeywordRecord::print(raw_ostream &out) const
{
    string s_type;
    switch(_type)
    {
    case CLASS: s_type = "in a Class"; break;
    case CLASSTEMPLATE: s_type = "in a Class-Template"; break;
    case COMMENT: s_type = "in a Comment"; break;
    case FIELD: s_type = "as a Field"; break;
    case FUNCTION: s_type = "in a Function"; break;
    case FUNCTIONTEMPLATE: s_type = "in a Function-Template"; break;
    case FILENAME: s_type = "in a Filename"; break;
    case NAMESPACE: s_type = "as a Namespace"; break;
    case VARIABLE: s_type = "as a Variable"; break;
    case VARTEMPLATE: s_type = "in a Variable-Template"; break;
    }

    string position;

    if(_type == FoundKeywordType::FILENAME){
        position = "\n";
    } else {
        position = " " + to_string(_line) + ":" + to_string(_col) + "\n";
    }

    out << "Found Keyword: "
        << _keyword << " "
        << s_type << " at\n  "
        << _fileName << position;
}

void FoundKeywordRecord::print() const
{
    print(llvm::outs());
}

string FoundKeywordRecord::fileName() const
{
    return _fileName;
}

void FoundKeywordRecord::setFileName(const string fileName)
{
    _fileName = fileName;
}

unsigned FoundKeywordRecord::line() const
{
    return _line;
}

void FoundKeywordRecord::setLine(const unsigned &line)
{
    _line = line;
}

unsigned FoundKeywordRecord::col() const
{
    return _col;
}

void FoundKeywordRecord::setCol(const unsigned &col)
{
    _col = col;
}

bool FoundKeywordRecord::less(const FoundKeywordRecord &fkr1, const FoundKeywordRecord &fkr2)
{
    if(fkr1.keyword().compare(fkr2.keyword()) < 0){
        return true;
    } else if(fkr1.keyword().compare(fkr2.keyword()) == 0 ){
        if(fkr1.fileName().compare(fkr2.fileName()) < 0){
            return true;
        } else if(fkr1.fileName().compare(fkr2.fileName()) == 0){
            if(fkr1.type() < fkr2.type()){
                return true;
            } else if(fkr1.type() == fkr2.type()
                      && fkr1.type() != FoundKeywordType::FILENAME){
                if(fkr1.line() < fkr2.line()){
                    return true;
                } else if(fkr1.line() == fkr2.line() && fkr1.col() < fkr2.col()){
                    return true;
                }// End else-if line and column
            }// End else-if type
        }// End else-if Filename
    }// End else-if Keyword
    return false;
}



