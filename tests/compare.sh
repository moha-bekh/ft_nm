#!/bin/sh
set -eu

if [ ! -x ./ft_nm ]; then
	make
fi

if [ ! -d tests/out ]; then
	sh tests/make_samples.sh >/dev/null
fi

status=0

for file in tests/out/basic64.o tests/out/basic32.o tests/out/basic64 tests/out/basic32 tests/out/libbasic64.so tests/out/libbasic32.so tests/out/undef64.o tests/out/undef32.o; do
	for opt in default -a -g -u -r -p; do
		printf '\n== %s %s ==\n' "$opt" "$file"
		if [ "$opt" = "default" ]; then
			./ft_nm "$file" > /tmp/ft_nm.out 2> /tmp/ft_nm.err || true
			nm "$file" > /tmp/nm.out 2> /tmp/nm.err || true
		else
			./ft_nm "$opt" "$file" > /tmp/ft_nm.out 2> /tmp/ft_nm.err || true
			nm "$opt" "$file" > /tmp/nm.out 2> /tmp/nm.err || true
		fi
		if ! diff -u /tmp/nm.out /tmp/ft_nm.out; then
			status=1
		fi
	done
done

printf '\n== invalid inputs: should not segfault or bus error ==\n'
for file in /dev/null Makefile tests/out/not_elf.txt tests/out/dir missing_file; do
	printf '\n-- %s --\n' "$file"
	./ft_nm "$file" >/tmp/ft_nm.invalid.out 2>/tmp/ft_nm.invalid.err || true
	cat /tmp/ft_nm.invalid.err
done

exit "$status"
