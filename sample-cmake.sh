#!/bin/sh
# usage: sample-cmake.sh [Release|Debug]
CMAKE_OPTIONS="-DCMAKE_BUILD_WITH_INSTALL_RPATH=True"
case "$1" in
    Debug|Release) CONFIG=$1 && shift ;;
                *) CONFIG=Debug ;;
esac
CMAKE_OPTIONS="${CMAKE_OPTIONS} -DCMAKE_BUILD_TYPE=${CONFIG}"
PREFIX=$HOME/local/llvm-mi
CMAKE_OPTIONS="${CMAKE_OPTIONS} -DCMAKE_INSTALL_PREFIX=${PREFIX}"
mkdir build-$CONFIG
cd build-$CONFIG
cmake ${CMAKE_OPTIONS} $* ..