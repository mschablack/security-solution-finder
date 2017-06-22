// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file implements the FindAPICalls class.

#include "findapicalls.h"

FindAPICalls::FindAPICalls() : _records(&APICallRecord::less),
    _builtFileRecordMap(false), _builtFileRatings(false)
{

}

void FindAPICalls::addAPICallRecord(const APICallRecord record)
{
    _records.insert(record);
}

void FindAPICalls::printAllRecords(raw_ostream &out) const
{
    for(APICallRecord record : _records)
    {
        record.print(out);
    }
}

void FindAPICalls::printResultsForFile(string file, raw_ostream &out)
{
    auto it = _fileRecordMap.find(file);
    if( it != _fileRecordMap.end())
    {
        map<string, set<string> > apiResultStrings;
        for(string apiName : _apiNames)
        {
            apiResultStrings[apiName] = set<string>();
        }
        for(APICallRecord record : it->second)
        {
            string recordResultString = to_string(record.line())
                    + ":" + to_string(record.col()) + " - Function: "
                    + record.funcName();
            apiResultStrings.at(record.apiName()).insert(recordResultString);
        }
        for(string apiName : _apiNames)
        {
            set<string> resultStrings = apiResultStrings.at(apiName);
            if(!resultStrings.empty())
            {
                out << " Found API-Call for API: " << apiName << " at:\n";
                for(string position : resultStrings)
                {
                    out << "  " << position << "\n";
                }
            }
        }
    }
}

void FindAPICalls::buildFileFoundRecordMapping()
{
    for(APICallRecord record : _records)
    {
        string fileName = record.fileName();
        auto it = _fileRecordMap.find(fileName);
        if(it != _fileRecordMap.end())
        {
            it->second.insert(record);
        } else
        {
            APICallRecordSet recordSet (&APICallRecord::less);
            recordSet.insert(record);
            _fileRecordMap[fileName] = recordSet;
        }
    }
    _builtFileRecordMap = true;
}

map<string, APICallRecordSet> FindAPICalls::getFileFoundRecordMap()
{
    if(!_builtFileRecordMap)
    {
        buildFileFoundRecordMapping();
    }
    return _fileRecordMap;
}

void FindAPICalls::buildFileRating()
{
    if(!_builtFileRecordMap)
    {
        buildFileFoundRecordMapping();
    }
    for(pair<string,APICallRecordSet> fileRecords : _fileRecordMap)
    {
        map<string,unsigned> proAPIRating;
        for(string apiName : _apiNames)
        {
            proAPIRating[apiName]=100;
        }
        for(APICallRecord record : fileRecords.second)
        {
            unsigned current = proAPIRating.at(record.apiName());
            proAPIRating[record.apiName()] =
                    min<unsigned>(current,getRecordRating(record));
        }
        double fileRating = 1.0;
        for(pair<string,unsigned> apiRating : proAPIRating)
        {
            fileRating = fileRating * ( (double) apiRating.second / 100.0);
        }
        _fileRating[fileRecords.first] = fileRating;
    }
    _builtFileRatings = true;
}

double FindAPICalls::getFileRating(string file)
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

unsigned FindAPICalls::getRecordRating(APICallRecord record)
{
    return _functionRating.at(record.funcName());
}

vector<string> FindAPICalls::getAPIFunctions() const
{
    return _funcNames;
}

void FindAPICalls::addAPIFunction(const string funcName, const string apiName,
                                  const unsigned rating)
{
    _funcNames.push_back(funcName);
    _mapFuncAPI[funcName] = apiName;
    _functionRating[funcName] = rating;
}

void FindAPICalls::addAPIHeader(const string api, const string header)
{
    _apiNames.insert(api);

    auto it = _mapAPIHeader.find(api);
    if(it == _mapAPIHeader.end())
    {
        _mapAPIHeader[api] = set<string>();
    }
    _mapAPIHeader.at(api).insert(header);
}

set<string> FindAPICalls::getAPIHeaders(string api) const
{
    return _mapAPIHeader.at(api);
}

string FindAPICalls::getAPIforFunc(const string funcName) const
{
    return _mapFuncAPI.at(funcName);
}
