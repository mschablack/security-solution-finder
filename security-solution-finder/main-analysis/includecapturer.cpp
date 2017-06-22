// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// Implementation of the IncludeCapturer.

#include "includecapturer.h"


void IncludeCapturer::InclusionDirective(SourceLocation HashLoc,
                                         const Token &IncludeTok,
                                         llvm::StringRef FileName,
                                         bool IsAngled,
                                         CharSourceRange FilenameRange,
                                         const FileEntry *File,
                                         llvm::StringRef SearchPath,
                                         llvm::StringRef RelativePath,
                                         const clang::Module *Imported)
{
    if(IncludeTok.isAnyIdentifier()
            && IncludeTok.getIdentifierInfo()->getName() == "include"
            && !PP.getSourceManager().isInSystemHeader(HashLoc))
    {
        PresumedLoc PLoc = PP.getSourceManager().getPresumedLoc(HashLoc);
        StringRef originFile = PLoc.getFilename();
        if(!originFile.startswith("<"))
        {
            string includeName = FileName.str();
            _sources->addSourceInclude(originFile.str(),includeName);
        }
    }
}

void IncludeCapturer::FileChanged(SourceLocation Loc,
                                  FileChangeReason Reason,
                                  SrcMgr::CharacteristicKind FileType,
                                  FileID PrevFID)
{
    if(Reason == FileChangeReason::EnterFile
            && !PP.getSourceManager().isInSystemHeader(Loc))
    {
        PresumedLoc PLoc = PP.getSourceManager().getPresumedLoc(Loc);
        StringRef fileName = PLoc.getFilename();
        if(!fileName.startswith("<"))
        {
            _sources->addSourceFile(fileName.str());
        }
    } else if(Reason == FileChangeReason::ExitFile
              && !PP.getSourceManager().isInSystemHeader(Loc))
    {
        const FileEntry* prevFile = PP.getSourceManager().getFileEntryForID(PrevFID);
        if(prevFile)
        {
            if(!PP.getSourceManager().isInSystemHeader(
                        PP.getSourceManager().getLocForStartOfFile(PrevFID)))
            {
                _sources->addIncludedFileForFile(
                            PP.getSourceManager().getPresumedLoc(Loc).getFilename(),
                            prevFile->getName().str());
            }
        }
    }
}
