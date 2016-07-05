#!/bin/sh

set -e

prefix="$(pwd)/release/mingw"
mkdir -p "$prefix"

builddir="$(pwd)/build_i386"
mkdir -p "$builddir"

build=mingw32

echo Building shared library...
mkdir -p m4
autoreconf -i
(cd "$builddir" &&
  CPPFLAGS='-DNDEBUG'	\
  CFLAGS='-O3 -ffunction-sections -fdata-sections'	\
  LDFLAGS='-Wl,-s,--gc-sections'	\
  ../configure --build="$build" --host="$build" --prefix="$prefix" && make -j1)

echo Testing...
cp -p "$builddir/libmcfgthread.dll.a" "$builddir/libmcfgthread-new.dll.a"
("$build"-gcc -std=c11 -Wall -Wextra -pedantic -Wno-error test/test.c -Isrc/env -L"$builddir" -lmcfgthread-new -o test.exe &&
  PATH="$builddir:$PATH" ./test.exe)

(cd "$builddir" &&
  make install)
