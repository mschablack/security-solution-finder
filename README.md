# security-solution-finder

## About
This is a prototype for detecting the security patterns "sandbox" and "encrypted storage" within programs.
The prototype is using the LibTooling interface of clang.

## Install

First you need to get the clang code. It is recommended to do it according to the instructions provided on the clang webpage: https://clang.llvm.org/get_started.html

Once you have got the clang code, place the the "security-solution-finder"-folder into:
LLVM.DIR/tools/clang/tools/extra
and add "add_subdirectory(security-solution-finder)" to the CMakeLists.txt in the folder LLVM.DIR/tools/clang/tools/extra.
Now you can build the security-solution-finder by using the following cmake command:
cmake --build . --target security-solution-finder

## Usage

security-solution-finder FILELIST -ofile=OUTPUTFILE -- CLANG-OPTIONS

## License
This porject uses the University of Illinios Open Source License.
Details can be found in the LICENSE file.