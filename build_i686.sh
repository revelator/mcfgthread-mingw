#!/bin/sh

set -e

prefix="$(pwd)/release/mingw32"
mkdir -p "$prefix"

builddir="$(pwd)/build_i686"
mkdir -p "$builddir"

build=i686-w64-mingw32

echo Building shared library...
mkdir -p m4
autoreconf -i
(cd "$builddir" &&
  CPPFLAGS='-DNDEBUG'	\
  CFLAGS='-O3 -ffunction-sections -fdata-sections'	\
  LDFLAGS='-Wl,-s,--gc-sections'	\
  ../configure --build="$build" --host="$build" --prefix="$prefix" && make -j4)

echo Testing...
cp -p "$builddir/libmcfgthread.dll.a" "$builddir/libmcfgthread-new.dll.a"
("$build"-gcc -std=c11 -Wall -Wextra -pedantic -Werror test/test.c -Isrc/env -L"$builddir" -lmcfgthread-new -o test.exe &&
  PATH="$builddir:$PATH" ./test.exe)

(cd "$builddir" &&
  make install)
