# clang-tidy-audit

A prototype tool that rewrites c/cpp/objc files to annotate security points of interest.

 * replaceUsesOfAuto - rewrite uses of auto to their deduced types
 * elaborateOnImplicitCasts - make implicit casts more explicit by injecting comments
 * annotateLambaPointerCaptures - inject comments anytime a pointer is captured by lambda 

## Prerequisites

This project builds clang and llvm and has the same prerequisites:

### Mac OS X
```
brew install cmake
```

### Ubuntu / Debian Linux
```
sudo apt-get build-dep clang-3.7
```

## Build

```
   ./build.sh
```

## Test

```
  ./test.sh
```

## Usage

clang-tidy is built on top of clang's libtooling interface and requires your project to have a compilation database. The easiest way to get a compliation database is to let cmake generate one for you with:
```
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON path_to_cmakelists
```

This creates a compile\_commands.json, which tells clang-tidy the specific compile commands used so it can properly parse the source file into an AST.

```
./build/bin/clang-tidy -checks=readability-AuditAnnotate file_to_check.cpp
```
