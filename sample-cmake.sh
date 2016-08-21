#!/bin/sh
#
# usage: sample-cmake.sh [Release|Debug] {any other cmake option}*
#
# any other cmake option may be
# -DCMAKE_INSTALL_PREFIX=
# -DLLDB_API_INCLUDE_DIR=
# -DLLDB_LIBRARY_DIR=

CMAKE_OPTIONS="-DCMAKE_BUILD_WITH_INSTALL_RPATH=True"
case "$1" in
    Debug|Release) CONFIG=$1 && shift ;;
                *) CONFIG=Debug ;;
esac
BUILDDIR="build-$CONFIG"
CMAKE_OPTIONS="${CMAKE_OPTIONS} -DCMAKE_BUILD_TYPE=${CONFIG}"
mkdir -p "$BUILDDIR"           && \
  cd "$BUILDDIR"               && \
  cmake ${CMAKE_OPTIONS} $* .. && \
  cd ..                        && \
  make -C "$BUILDDIR"
