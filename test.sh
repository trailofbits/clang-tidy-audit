#!/bin/bash

set -v

mkdir -p test/build
pushd test/build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
make
popd

pushd test
ln -s build/compile_commands.json compile_commands.json
../build/bin/clang-tidy -checks=readability-Auditannotate test.cpp
../build/bin/clang-tidy -checks=readability-AuditAnnotate test2.cpp
popd


