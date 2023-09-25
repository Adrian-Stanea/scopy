#!/bin/bash
set -x

SCOPY_REPO=$(git rev-parse --show-toplevel)

IN_PLACE="-i"
VERBOSE="--verbose"

find $SCOPY_REPO -type f -name "*.cpp" | xargs clang-format $VERBOSE $IN_PLACE
find $SCOPY_REPO -type f -name "*.cc" | xargs clang-format $VERBOSE $IN_PLACE
find $SCOPY_REPO -type f -name "*.hpp" | xargs clang-format $VERBOSE $IN_PLACE
find $SCOPY_REPO -type f -name "*.h" | xargs clang-format $VERBOSE $IN_PLACE
find $SCOPY_REPO -type f -name "CMakeLists.txt" | xargs cmake-format $IN_PLACE
find $SCOPY_REPO -type f -name "*.cmake" | xargs cmake-format $IN_PLACE
pushd $SCOPY_REPO/tools
./includemocs.sh
popd

echo Formatting complete

