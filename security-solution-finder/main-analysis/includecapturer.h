// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// Implementation of the PPCallbacks interface to capture all include statements
// within a translation unit.

#ifndef INCLUDECAPTURER_H
#define INCLUDECAPTURER_H

#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"
#include "main-analysis/sources.h"

using namespace clang;
using namespace llvm;
using namespace std;

class IncludeCapturer : public PPCallbacks
{
public:
    explicit IncludeCapturer(Preprocessor &pp, shared_ptr<Sources> sources)
        : PP(pp), _sources(sources) {}

    void InclusionDirective(SourceLocation HashLoc,
                            const Token &IncludeTok,
                            StringRef FileName,
                            bool IsAngled,
                            CharSourceRange FilenameRange,
                            const FileEntry *File,
                            StringRef SearchPath,
                            StringRef RelativePath,
                            const clang::Module *Imported) override;

    void FileChanged(SourceLocation Loc, FileChangeReason Reason,
                     SrcMgr::CharacteristicKind FileType,
                     FileID PrevFID) override;

private:
    Preprocessor &PP;
    shared_ptr<Sources> _sources;
};

#endif // INCLUDECAPTURER_H
