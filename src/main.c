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


static int nm(char *filename, t_options *options) {

  t_nm nm = {0};
  nm.options = options;

  if (map_elf(&nm, filename))
    return NO_EXIT_FAILURE;

  if (parse_elf(&nm, filename)) {
    munmap(nm.file_ptr, nm.file_size);
    return NO_EXIT_FAILURE;
  }

  if (filter_symbols(&nm)) {
    munmap(nm.file_ptr, nm.file_size);
    return NO_EXIT_FAILURE;
  }

  sort_symbols(&nm);

  if (nm.options->rev_sort) {
    dlst_foreach_reverse(nm.symbols, nm.print);
  } else {
    dlst_foreach(nm.symbols, nm.print);
  }

  dlst_clear(&nm.symbols, free);
  munmap(nm.file_ptr, nm.file_size);
  return EXIT_SUCCESS;
}

static int count_files(int ac, char **av, t_options *options)
{
    int count = 0;
    bool end_opts = false;

    for (int i = 1; i < ac; i++)
    {
        if (av[i][0] == '-' && av[i][1] != '\0' && !end_opts)
        {
            if (ft_strcmp(av[i], "--") == 0)
                end_opts = true;
        }
        else
            count++;
    }
    return (count);
}

int main(int ac, char **av) {

  t_options options = {0};
  parse_options(ac, av, &options);

  int file_count = count_files(ac, av, &options);
  if (file_count == 0)
  {
      if (nm("a.out", &options) != EXIT_SUCCESS)
          return (EXIT_FAILURE);
      return (EXIT_SUCCESS);
  }

  int _errno = 0;

  for (int i = 1; i < ac; i++) {
    if (av[i][0] == '-' && options.end == false) {
      if (!ft_strcmp(av[i], "--")) {
        options.end = true;
      }
    } else {
      if (file_count > 1)
        ft_dprintf(STDOUT_FILENO, "\n%s:\n", av[i]);
      int ret = nm(av[i], &options);
      if (ret == EXIT_FAILURE)
        _errno += 1;
      else if (ret == NO_EXIT_FAILURE) {
        _errno += 1;
      }
    }
  }
  return _errno;
}
