#!/bin/bash

git clone git@github.com:llvm-mirror/llvm.git -b release_37
git clone git@github.com:llvm-mirror/clang.git llvm/tools/clang -b release_37
git clone git@github.com:llvm-mirror/clang-tools-extra.git llvm/tools/clang/tools/extra -b release_37
git clone git@github.com:llvm-mirror/compiler-rt llvm/projects/compiler-rt -b release_37
git clone git@github.com:llvm-mirror/libcxx.git llvm/projects/libcxx -b release_37
git clone git@github.com:llvm-mirror/libcxxabi llvm/projects/libcxxabi -b release_37

pushd llvm/tools/clang/tools/extra/clang-tidy
python add_new_check.py readability Auditannotate
popd

# This overwrites the autogenerated ones
cp AuditannotateCheck.cpp AuditannotateCheck.h llvm/tools/clang/tools/extra/clang-tidy/readability/

mkdir build
pushd build
cmake ../llvm
make
popd

