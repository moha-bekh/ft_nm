# ft_nm development environment

This repository is prepared for the 42 `ft_nm` project on a Mac M1 host.
The actual work happens inside a Debian `linux/amd64` container so generated
and inspected files are ELF, not Mach-O.

## First run

```sh
task build
task up
```

Inside the container:

```sh
make
sh tests/make_samples.sh
readelf -h -S -s tests/out/basic64.o
nm tests/out/basic64.o
```

From another terminal, while the container is running:

```sh
task samples
task compare
task valgrind FILE=tests/out/basic64.o
```

## Useful comparisons

```sh
./ft_nm tests/out/basic64.o > /tmp/ft
nm tests/out/basic64.o > /tmp/ref
diff -u /tmp/ref /tmp/ft
```

Bonus options required by the subject:

```sh
for opt in "" "-a" "-g" "-u" "-r" "-p"; do
	./ft_nm $opt tests/out/basic64.o > /tmp/ft
	nm $opt tests/out/basic64.o > /tmp/ref
	diff -u /tmp/ref /tmp/ft
done
```

Invalid inputs to keep in the loop:

```sh
./ft_nm /dev/null
./ft_nm Makefile
./ft_nm tests/out/not_elf.txt
./ft_nm tests/out/dir
./ft_nm missing_file
```

## Layout

```text
.
|-- Dockerfile
|-- Taskfile.yaml
|-- Makefile
|-- include/
|-- src/
`-- tests/
    |-- samples/
    |-- make_samples.sh
    `-- compare.sh
```
