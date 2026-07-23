#ifndef FT_NM_H
#define FT_NM_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>
#include <elf.h>
// #include <fcntl.h>
// #include <stdio.h>
#include <sys/mman.h>
// #include <unistd.h>

#include "libft.h"
#include "dlst.h"

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
  char *type;
} t_symbol;

enum e_elf_class {
  Elf32 = 1,
  Elf64 = 2
};

typedef struct s_nm {
  void *file_ptr;
  size_t file_size;
  t_dlst *symbols;
  size_t sym_count;
  enum e_elf_class elf_class;
} t_nm;

// typedef struct s_symbol {
//   char *name;
//   unsigned long value;
//   char type;
//   unsigned char bind;
//   unsigned char sym_type;
//   unsigned short shndx;
// } t_symbol;
//
// typedef struct s_elf {
//   Elf64_Ehdr *ehdr; // elf header
//   Elf64_Shdr *shdr; // section header table
//   Elf64_Shdr
//       *shstrtab_hdr; // section header for the section header string table
//   char *shstrtab;    // ptr to section header string table
//   Elf64_Sym *symtab; // symbol table
//   long symtab_size;  // size of symbol table
//   char *strtab;      // string table
// } t_elf;
//
// typedef struct s_nm {
//   int fd;
//   struct stat stat;
//   void *file_ptr;
//   t_elf elf;
//   t_symbol *syms;
//   long symnum;
// } t_nm;

#endif /* FT_NM_H */
