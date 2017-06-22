// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file implements the FindKeywords class of the Find-Keyword criterion.

#include "findkeywords.h"

vector<string> FindKeywords::getKeywords()
{
    return _keywords;
}

void FindKeywords::addKeyword(string keyword, unsigned rating, bool actionbased)
{
    _keywords.push_back(keyword);
    _keywordRating[keyword] = rating;
    _keywordActionbasedMap[keyword] = actionbased;
}

void FindKeywords::addFoundKeyword(const FoundKeywordRecord keyword)
{
    _foundKeywords.insert(keyword);
}

void FindKeywords::printAllFoundKeywords(raw_ostream &out) const
{
    for(FoundKeywordRecord fkr : _foundKeywords)
    {
        fkr.print(out);
    }
}

void FindKeywords::printResultsForFile(string file, raw_ostream &out)
{
    auto it = _fileRecordMap.find(file);
    if( it != _fileRecordMap.end())
    {
        map<string, set<string> > keywordResultStrings;
        for(string keyword : _keywords)
        {
            keywordResultStrings[keyword] = set<string>();
        }
        for(FoundKeywordRecord record : it->second)
        {
            string recordResultString = to_string(record.line())
                    + ":" + to_string(record.col()) + " ("
                    + record.typeAsString() + "), ";
            keywordResultStrings.at(record.keyword()).insert(recordResultString);
        }
        for(string keyword : _keywords)
        {
            set<string> resultStrings = keywordResultStrings.at(keyword);
            if(!resultStrings.empty())
            {
                out << " Found Keyword: " << keyword << " at: ";
                for(string position : resultStrings)
                {
                    out << position;
                }
                out << "\n";
            }
        }
    }
}

void FindKeywords::setSourceFiles(shared_ptr<Sources> sources)
{
    _sources = sources;
}

void FindKeywords::analyzeFilenames()
{
    for(StringRef filename : _sources->sources())
    {
        for(StringRef keyword : _keywords)
        {
            if(filename.contains_lower(keyword)) {
                FoundKeywordRecord fkr (keyword.str(), filename.str(),
                                        FoundKeywordType::FILENAME);
                fkr.print();
                addFoundKeyword(fkr);
            }
        }
    }
}

void FindKeywords::buildFileFoundRecordMapping()
{
    for(FoundKeywordRecord fr : _foundKeywords)
    {
        string fileName = fr.fileName();
        auto it = _fileRecordMap.find(fileName);
        if(it == _fileRecordMap.end())
        {
            _fileRecordMap[fileName] = KeywordRecordSet(&FoundKeywordRecord::less);
        }
        _fileRecordMap.at(fileName).insert(fr);
    }
    _builtFileRecordMap = true;
}

map<string, KeywordRecordSet> FindKeywords::getFileFoundRecordMap()
{
    if(!_builtFileRecordMap)
    {
        buildFileFoundRecordMapping();
    }
    return _fileRecordMap;
}

void FindKeywords::buildFileRating()
{
    if(!_builtFileRecordMap)
    {
        buildFileFoundRecordMapping();
    }
    for(pair<string,KeywordRecordSet> fileRecords : _fileRecordMap)
    {
        map<string,unsigned> proKeywordRating;
        for(string keyword : getKeywords())
        {
            proKeywordRating[keyword]=100;
        }
        for(FoundKeywordRecord record : fileRecords.second)
        {
            unsigned current = proKeywordRating.at(record.keyword());
            proKeywordRating[record.keyword()] =
                    min<unsigned>(current,getRecordRating(record));
        }
        double fileRating = 1.0;
        for(pair<string,unsigned> keywordRating : proKeywordRating)
        {
            fileRating = fileRating * ( (double) keywordRating.second / 100.0);
        }
        _fileRating[fileRecords.first] = fileRating;
    }
    _builtFileRatings = true;
}

double FindKeywords::getFileRating(string file)
{
    if(!_builtFileRatings)
    {
        buildFileRating();
    }
    auto it = _fileRating.find(file);
    if(it == _fileRating.end())
    {
        return 1.0;
    }
    return it->second;
}

unsigned FindKeywords::getRecordRating(FoundKeywordRecord record)
{
    unsigned positionRating;
    if(isActionBased(record))
    {
        positionRating = getActionBasedFoundTypeRating(record.type());
    } else {
        positionRating = getStructureBasedFoundTypeRating(record.type());
    }
    return positionRating * _keywordRating.at(record.keyword());
}

bool FindKeywords::isActionBased(FoundKeywordRecord record)
{
    return _keywordActionbasedMap[record.keyword()];
}

unsigned FindKeywords::getActionBasedFoundTypeRating(FoundKeywordType type)
{
    unsigned rating;
    switch(type)
    {
    case CLASS: rating = 3; break;
    case CLASSTEMPLATE: rating = 3; break;
    case COMMENT: rating = 10; break;
    case FIELD: rating = 6; break;
    case FUNCTION: rating = 1; break;
    case FUNCTIONTEMPLATE: rating = 1; break;
    case FILENAME: rating = 1; break;
    case NAMESPACE: rating = 8; break;
    case VARIABLE: rating = 2; break;
    case VARTEMPLATE: rating = 2; break;
    }
    return rating;
}

unsigned FindKeywords::getStructureBasedFoundTypeRating(FoundKeywordType type)
{
    unsigned rating;
    switch(type)
    {
    case CLASS: rating = 1; break;
    case CLASSTEMPLATE: rating = 1; break;
    case COMMENT: rating = 10; break;
    case FIELD: rating = 2; break;
    case FUNCTION: rating = 4; break;
    case FUNCTIONTEMPLATE: rating = 4; break;
    case FILENAME: rating = 1; break;
    case NAMESPACE: rating = 1; break;
    case VARIABLE: rating = 4; break;
    case VARTEMPLATE: rating = 4; break;
    }
    return rating;
}
