#!/bin/sh
set -eu

OUT_DIR="tests/out"

mkdir -p "$OUT_DIR"

cc -c tests/samples/basic.c -o "$OUT_DIR/basic64.o"
cc -m32 -c tests/samples/basic.c -o "$OUT_DIR/basic32.o"
cc tests/samples/basic.c -o "$OUT_DIR/basic64"
cc -m32 tests/samples/basic.c -o "$OUT_DIR/basic32"
cc -shared -fPIC tests/samples/basic.c -o "$OUT_DIR/libbasic64.so"
cc -m32 -shared -fPIC tests/samples/basic.c -o "$OUT_DIR/libbasic32.so"
cc -c tests/samples/undef.c -o "$OUT_DIR/undef64.o"
cc -m32 -c tests/samples/undef.c -o "$OUT_DIR/undef32.o"

printf 'not an elf\n' > "$OUT_DIR/not_elf.txt"
mkdir -p "$OUT_DIR/dir"

file "$OUT_DIR"/*
