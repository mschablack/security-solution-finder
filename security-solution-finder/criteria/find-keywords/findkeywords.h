// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file defines the FindKeywords class, which is the main class for the
// Find-Keywords criterion. All information needed to search for Keywords
// are stored here, as well as all found keywords.

#ifndef FINDKEYWORDS_H
#define FINDKEYWORDS_H

#include <set>
#include <map>
#include "clang/Tooling/Tooling.h"
#include "criteria/find-keywords/foundkeywordrecord.h"
#include "main-analysis/sources.h"


using namespace std;
using namespace clang;
using namespace llvm;


typedef std::set<FoundKeywordRecord, bool(*)(
        const FoundKeywordRecord&,const FoundKeywordRecord&)> KeywordRecordSet;

class FindKeywords
{
public:
    FindKeywords(shared_ptr<Sources> sources)
        : _sources(sources), _foundKeywords(&FoundKeywordRecord::less),
          _builtFileRecordMap(false), _builtFileRatings(false) {}

    vector<string> getKeywords();
    void addKeyword(string keyword, unsigned rating, bool actionbased);
    void addFoundKeyword(const FoundKeywordRecord keyword);
    void printAllFoundKeywords(raw_ostream &out) const;
    void printResultsForFile(string file, raw_ostream &out);
    void setSourceFiles(shared_ptr<Sources> sources);
    void analyzeFilenames();
    void buildFileFoundRecordMapping();
    map<string, KeywordRecordSet> getFileFoundRecordMap();

    void buildFileRating();
    double getFileRating(string file);

    unsigned getRecordRating(FoundKeywordRecord record);
    bool isActionBased(FoundKeywordRecord record);
    unsigned getActionBasedFoundTypeRating(FoundKeywordType type);
    unsigned getStructureBasedFoundTypeRating(FoundKeywordType type);

private:
    vector<string> _keywords;
    map<string, unsigned> _keywordRating;
    map<string, bool> _keywordActionbasedMap;
    shared_ptr<Sources> _sources;
    KeywordRecordSet _foundKeywords;
    map<string, KeywordRecordSet> _fileRecordMap;
    bool _builtFileRecordMap;
    map<string,double> _fileRating;
    bool _builtFileRatings;
};

#endif // FINDKEYWORDS_H
