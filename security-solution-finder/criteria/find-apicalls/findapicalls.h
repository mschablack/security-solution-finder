// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file defines the FindAPICalls class, which is the main class for the
// Find-API criterion. All information needed to search for API-Calls
// are stored here, as well as all found calls.

#ifndef FINDAPICALLS_H
#define FINDAPICALLS_H

#include <map>
#include <set>
#include "clang/Tooling/Tooling.h"
#include "criteria/find-apicalls/apicallrecord.h"

using namespace std;
using namespace clang;
using namespace llvm;

typedef set<APICallRecord,bool(*)(
        const APICallRecord&,const APICallRecord&)> APICallRecordSet;

class FindAPICalls
{
public:
    FindAPICalls();

    void addAPICallRecord(const APICallRecord record);
    void printAllRecords(raw_ostream &out) const;
    void printResultsForFile(string file, raw_ostream &out);

    void buildFileFoundRecordMapping();
    map<string, APICallRecordSet> getFileFoundRecordMap();
    void buildFileRating();
    double getFileRating(string file);

    unsigned getRecordRating(APICallRecord record);

    vector<string> getAPIFunctions() const;

    void addAPIFunction(const string funcName, const string apiName,
                        const unsigned rating);

    void addAPIHeader(const string api, const string header);

    set<string> getAPIHeaders(string api) const;

    string getAPIforFunc(const string funcName) const;

private:
    vector<string> _funcNames;
    map<string, unsigned> _functionRating;
    set<string> _apiNames;
    map<string, string> _mapFuncAPI;
    map<string, set<string> > _mapAPIHeader;
    APICallRecordSet _records;
    map<string, APICallRecordSet> _fileRecordMap;
    bool _builtFileRecordMap;
    map<string,double> _fileRating;
    bool _builtFileRatings;
};

#endif // FINDAPICALLS_H
