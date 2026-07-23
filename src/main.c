/*

ft_nm:
  - A simple implementation of the nm command for ELF files.
    It lists symbols from object files.

  - e.g ./ft_nm -g -u -r ft_nm

  - options priority:
    filter: -u > -g > -a
    sort: -p > -r

Authorized functions:
  - open, close, mmap, munmap, write, fstat, 
    malloc, free, exit, perror, strerror, getpagesize
    libft

I/O validation:
  - file format - ELF
  - Architecture - ELF32 | ELF64
  - file type - executable, .o, .so

Filter:
  -a -  All symbols are listed, even if they are not defined in the object file.
          debugger-only symbols | STT_FILE

  -g -  Only external (global) symbols are listed.
          Maj | STB_GLOBAL | STB_WEAK

  -u - Only undefined symbols are listed.
          STB_WEAK | STT_NOTYPE 

Sort:
  - alpha
  -r -  Reverse sort
  -p -  No sort

Display

*/

#include "ft_nm.h"

void parse_options(int ac,
          char **av, 
          t_options *options
){
  for (int i = 1; i < ac; i++) {
    if (!ft_strcmp(av[i], "--")) {
      return ;
    }
    else if (av[i][0] == '-' && av[i][1]) {
      for (int j = 1; av[i][j]; j++) {
        switch (av[i][j]) {
          case 'a':
            options->all = true;
            break;
          case 'g':
            options->global_only = true;
            break;
          case 'u':
            options->undefined_only = true;
            break;
          case 'r':
            options->rev_sort = true;
            break;
          case 'p':
            options->no_sort = true;
            break;
          default:
            ft_dprintf(STDERR_FILENO,
                "ft_nm: invalid option -- '%c'\n"
                "Usage: ft_nm [option(s)] [file(s)]\n"
                " List symbols in [file(s)] (a.out by default).\n"
                " The options are:\n"
                "  -a  Display debugger-only symbols\n"
                "  -g  Display only external symbols\n"
                "  -p  Do not sort the symbols\n"
                "  -r  Reverse the order of the sort\n"
                "  -u  Display only undefined symbols\n",
                av[i][j]
            );
            exit(EXIT_FAILURE);
        }
      }
    }
  }
};

void print_debug_options(t_options *options) {
  ft_dprintf(STDERR_FILENO, "a[%d] g[%d] u[%d] r[%d] p[%d]\n",
      options->all,
      options->global_only,
      options->undefined_only,
      options->rev_sort,
      options->no_sort
  );
}

int nm(char *filename, t_options *options, int *_errno) {

  t_nm nm = {0};

  int fd = open(filename, O_RDONLY);
  if (fd < 0) {
    ft_dprintf(STDERR_FILENO, "ft_nm: '%s': No such file\n", filename);
    *_errno += 1;
    return EXIT_SUCCESS;
  }

  struct stat st;
  if (fstat(fd, &st) < 0) {
    ft_dprintf(STDERR_FILENO, "ft_nm: fstat: failed '%s'\n", filename);
    *_errno += 1;
    close(fd);
    return EXIT_SUCCESS;
  }

  // Check if the file is readable
  if (!(st.st_mode & (S_IRUSR | S_IRGRP | S_IROTH))) {
    *_errno += 1;
    return EXIT_SUCCESS;
  } 

  if ((long unsigned int)st.st_size < sizeof(Elf64_Ehdr)) {
    fprintf(stderr, "ft_nm: File is too small to be a valid ELF file\n");
    close(fd);
    return EXIT_SUCCESS;
  }

  nm.file_size = st.st_size;

  nm.file_ptr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (nm.file_ptr == MAP_FAILED) {
    perror("mmap");
    close(fd);
    return EXIT_FAILURE;
  }

  Elf64_Ehdr *ehdr = (Elf64_Ehdr *)nm.file_ptr;

  if (ehdr->e_ident[0] != ELFMAG0 || ehdr->e_ident[1] != ELFMAG1 ||
      ehdr->e_ident[2] != ELFMAG2 || ehdr->e_ident[3] != ELFMAG3) {
    fprintf(stderr, "ft_nme: %s: file format not recognized\n", filename);
    munmap(nm.file_ptr, st.st_size);
    close(fd);
    return EXIT_SUCCESS;
  }

  if (ehdr->e_ident[EI_CLASS] != ELFCLASS64) {
    fprintf(stderr, "File is not a 64-bit ELF file\n");
    munmap(nm.file_ptr, st.st_size);
    close(fd);
    return (1);
  }

  munmap(nm.file_ptr, st.st_size);
  close(fd);
  return EXIT_SUCCESS;
}

int main(int ac, char **av) {

  t_options options = {0};
  parse_options(ac, av, &options);

  int _errno = 0;

  for (int i = 1; i < ac; i++) {
    if (av[i][0] == '-' && options.end == false) {
      if (!ft_strcmp(av[i], "--")) {
        options.end = true;
      }
      continue ;
    } else {
      if (nm(av[i], &options, &_errno))
        return _errno;
    }
  }
  return _errno;
}
