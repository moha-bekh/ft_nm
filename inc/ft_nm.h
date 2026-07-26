#ifndef FT_NM_H
#define FT_NM_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>
#include <elf.h>
#include <sys/mman.h>

#include "libft.h"
#include "dlst.h"

#define NO_EXIT_FAILURE 2

typedef struct s_options {
  bool all;
  bool global_only;
  bool undefined_only;
  bool rev_sort;
  bool no_sort;
  bool end; // of options
} t_options;

typedef struct s_symbol {
  char *name;
  uint64_t value;
  char type;
} t_symbol;

enum e_elf_class {
  Elf32 = 1,
  Elf64 = 2
};

typedef struct s_nm {
  void *file_ptr;
  size_t file_size;
  t_dlst *symbols;

  enum e_elf_class elf_class;

  int (*parse)(struct s_nm *nm);
  void (*print)(void *symbol);

  t_options *options;
} t_nm;

void parse_options(int ac, char **av, t_options *options);

int map_elf(t_nm *nm, char *filename);
int parse_elf(t_nm *nm, char *filename);
int elf_64(t_nm *nm);
int elf_32(t_nm *nm);
//
int filter_symbols(t_nm *nm);

int sort_symbols(t_nm *nm);

void print_64_sym(void *symbol);
void print_32_sym(void *symbol);

#endif /* FT_NM_H */
