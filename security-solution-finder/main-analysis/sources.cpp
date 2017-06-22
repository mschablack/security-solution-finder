// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// Implementation of the Sources class, see sources.h

#include "sources.h"

Sources::Sources(vector<string> sources)
{
    addSourceFiles(sources);
}

void Sources::addSourceFiles(vector<string> sources)
{
    for(string source : sources)
    {
        _sources.insert(source);
    }
}

void Sources::addSourceFile(string file)
{
    _sources.insert(file);
}

void Sources::addSourceInclude(string sourceFile, string include)
{
    auto it = _directIncludes.find(sourceFile);
    if(it == _directIncludes.end())
    {
        _directIncludes[sourceFile] = set<string>();
    }
    _directIncludes.at(sourceFile).insert(include);
}

void Sources::addIncludedFileForFile(string sourceFile, string includedFile)
{
    auto it = _mapFileIncludedFile.find(sourceFile);
    if(it == _mapFileIncludedFile.end())
    {
        _mapFileIncludedFile[sourceFile] = set<string>();
    }
    _mapFileIncludedFile.at(sourceFile).insert(includedFile);
}

set<string> Sources::sources() const
{
    return _sources;
}

set<string> Sources::getAllAvailableIncludesForFile(string source)
{
    set<string> allIncludes;
    auto it = _allAvailableIncludes.find(source);
    if(it == _allAvailableIncludes.end())
    {
        allIncludes = getDirectIncludesForFile(source);
        // needs to be put twice into _allAvailableIncludes to prevent endless
        // recursion when header include each other
        _allAvailableIncludes[source] = allIncludes;
        for(string includedFile : getIncludedFilesForFile(source))
        {
            set<string> upperIncludes = getAllAvailableIncludesForFile(includedFile);
            allIncludes.insert(upperIncludes.begin(), upperIncludes.end());
        }
        _allAvailableIncludes[source] = allIncludes;
    } else {
        allIncludes = it->second;
    }
    return allIncludes;
}

set<string> Sources::getDirectIncludesForFile(string source) const
{
    auto it = _directIncludes.find(source);
    if(it == _directIncludes.end())
    {
        return set<string>();
    }
    return _directIncludes.at(source);
}

set<string> Sources::getIncludedFilesForFile(string sourceFile)
{
    auto it = _mapFileIncludedFile.find(sourceFile);
    if(it == _mapFileIncludedFile.end())
    {
        return set<string>();
    }
    return _mapFileIncludedFile.at(sourceFile);
}

void Sources::printAll()
{
    llvm::outs() << "Sourcefiles:\n";
    for(string source : _sources)
        llvm::outs() << source << "\n";

    llvm::outs() << "Conatining Headers:";
    for (auto const& x : _directIncludes)
    {
        llvm::outs() << "\n File:" << x.first << "\n Headers:\n";
        for (auto header : x.second)
            llvm::outs() << header << " ";
    }

    llvm::outs() << "Header File Mapping:\n";
    for (auto const& x : _mapFileIncludedFile)
    {
        llvm::outs() << "File:" << x.first << "\nIncluded Files:\n";
        for (auto header : x.second)
            llvm::outs() << " " << header << "\n";
    }
}
