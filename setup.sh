#!/bin/bash
run_ctest=${1:-1}       # run ctest if all else succeeds?   Defaults to Yes (1 > 0).
num_threads=${2:-8}     # num threads for make -j.  For CircleCI, try using 4.
echo "make will use $num_threads threads"

if (( $run_ctest > 2 )); then
    echo "Trying to run tests before re-building..."
    cd build && ctest && cd .. || exit 4  # run unit tests or die
fi

source scripts/set_env.sh

# cd externals
# git submodule update --init
# cd ..

#if the CMakeCache.txt file exists, then remove it and build cleanly
if [ -f CMakeCache.txt ]; then
    rm -f CMakeCache.txt
fi

#if the build directory exists, then remove it and build cleanly
if [ -d build ]; then
    rm -rf build
fi

mkdir -p build; cd build
cmake ..                || exit 1 #-DCMAKE_BUILD_TYPE=Debug
make  -j $num_threads   || exit 2
if (( $run_ctest > 0 )); then
    ctest               || exit 3  # run all unit tests
fi
