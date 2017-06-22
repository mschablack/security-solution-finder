// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file defines the Sources class, which is used to keep track of all
// files used in the analysis.

#ifndef SOURCES_H
#define SOURCES_H

#include <map>
#include <set>
#include <vector>
#include "llvm/Support/raw_ostream.h"

using namespace std;
using namespace llvm;

class Sources
{
public:
    Sources(vector<string> sources);

    void addSourceFiles(vector<string> sources);
    void addSourceFile(string file);

    void addSourceInclude(string sourceFile, string include);
    void addIncludedFileForFile(string sourceFile, string includedFile);

    set<string> sources() const;
    set<string> getDirectIncludesForFile(string source) const;
    set<string> getAllAvailableIncludesForFile(string source);
    set<string> getIncludedFilesForFile(string sourceFile);

    void printAll();

private:
    set<string> _sources;
    map<string,set<string> > _directIncludes;
    map<string,set<string> > _allAvailableIncludes;
    map<string,set<string> > _mapFileIncludedFile;
};

#endif // SOURCES_H
