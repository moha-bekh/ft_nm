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

char get_symbol_type(Elf64_Sym *sym, Elf64_Shdr *shdr)
{
    unsigned char bind = ELF64_ST_BIND(sym->st_info);
    unsigned char type = ELF64_ST_TYPE(sym->st_info);
    char          c;

    // 1. Gestion des Weak Symbols
    if (bind == STB_WEAK)
    {
        c = (type == STT_OBJECT) ? 'V' : 'W';
        if (sym->st_shndx == SHN_UNDEF)
            c = (type == STT_OBJECT) ? 'v' : 'w';
        return (c);
    }

    // 2. Index de sections spéciaux
    if (sym->st_shndx == SHN_UNDEF)
        return ('U');
    if (sym->st_shndx == SHN_ABS)
        c = 'A';
    else if (sym->st_shndx == SHN_COMMON)
        c = 'C';
    else
    {
        // 3. Déduction selon les flags de la section pointee par st_shndx
        Elf64_Word sh_type  = shdr[sym->st_shndx].sh_type;
        Elf64_Xword sh_flags = shdr[sym->st_shndx].sh_flags;

        if (sh_type == SHT_NOBITS && (sh_flags & SHF_ALLOC))
            c = 'B'; // .bss
        else if (sh_type == SHT_PROGBITS && (sh_flags & SHF_EXECINSTR))
            c = 'T'; // .text (code exécutable)
        else if (sh_flags & SHF_WRITE)
            c = 'D'; // .data (données mutables)
        else if (sh_flags & SHF_ALLOC)
            c = 'R'; // .rodata (lecture seule)
        else
            c = 'N'; // Debug info ou section non alloc
    }

    // 4. Si le binding est LOCAL, passer en minuscule (ex: T -> t, D -> d)
    if (bind == STB_LOCAL && c >= 'A' && c <= 'Z')
        c += 32;

    return (c);
}

int elf_64(t_nm *nm) {

  // elf header
  Elf64_Ehdr *ehdr = (Elf64_Ehdr *)nm->file_ptr;
  // section header table
  Elf64_Shdr *shdr = (Elf64_Shdr *)((char *)nm->file_ptr + ehdr->e_shoff);
  // section header for the section header string table
  Elf64_Shdr *shstrtab_hdr = &shdr[ehdr->e_shstrndx];
  // ptr to section header string table
  char *shstrtab = (char *)nm->file_ptr + shstrtab_hdr->sh_offset;

  // go through section headers to find symbol table
  for (int i = 0; i < ehdr->e_shnum; i++) {
    if (shdr[i].sh_type != SHT_SYMTAB)
      continue;

    // symbol table
    Elf64_Sym *symtab = (Elf64_Sym *)((char *)nm->file_ptr + shdr[i].sh_offset);
    int sym_count = shdr[i].sh_size / shdr[i].sh_entsize;
    unsigned int section_link = shdr[i].sh_link;
    // string table
    char *strtab = (char *)nm->file_ptr + shdr[section_link].sh_offset;

    // symtab[0] == STN_UNDEF (is the null symbol, skip it)
    for (int j = 1; j < sym_count; j++) {

      unsigned char st_type = ELF64_ST_TYPE(symtab[j].st_info);
      if (!nm->options->all) {
        if (st_type == STT_FILE || st_type == STT_SECTION)
          continue;
      }

      t_symbol *symbol = malloc(sizeof(t_symbol));
      if (!symbol) {
        ft_dprintf(STDERR_FILENO, "ft_nm: malloc failed\n");
        return EXIT_FAILURE;
      }

      symbol->value = symtab[j].st_value;
      symbol->type = get_symbol_type(&symtab[j], shdr);
      symbol->name = strtab + symtab[j].st_name;

      if (!dlst_push_back(&nm->symbols, symbol)) {
        ft_dprintf(STDERR_FILENO, "ft_nm: dlst_push_back failed\n");
        return EXIT_FAILURE;
      }
    }
  }
  return EXIT_SUCCESS;
}

int elf_32(t_nm *nm) {
  ft_dprintf(STDERR_FILENO, "ft_nm: 32-bit ELF files are not supported yet\n");
  return EXIT_SUCCESS;
}

int is_external_symbol(char type)
{
  if (type >= 'A' && type <= 'Z')
      return (1);

  if (type == 'w')
      return (1);

  return (0);
}

int filter_symbols(t_nm *nm) {
  t_dlst_node *node = dlst_head(nm->symbols);
  t_dlst_node *next;

  while (node) {
    t_symbol *symbol = (t_symbol *)dlst_data(node);
    next = dlst_next(node);

    bool keep = true;

    if (nm->options->undefined_only && symbol->type != 'U' && symbol->type != 'w')
      keep = false;

    if (nm->options->global_only && !is_external_symbol(symbol->type))
      keep = false;

    if (keep == false) {
      if (!dlst_del(&nm->symbols, node, free)) {
        ft_dprintf(STDERR_FILENO, "ft_nm: dlst_del failed\n");
        return EXIT_FAILURE;
      }
    }
    node = next;
  }
  return EXIT_SUCCESS;
}

int sort_alpha(void *a, void *b) {
  t_symbol *sym1 = (t_symbol *)a;
  t_symbol *sym2 = (t_symbol *)b;

  if (!sym1 || !sym1->name) return (-1);
  if (!sym2 || !sym2->name) return (1);

  return (ft_strcmp(sym1->name, sym2->name));
}

int sort_symbols(t_nm *nm) {
  if (nm->options->no_sort) {
    return EXIT_SUCCESS;
  }
  dlst_sort(nm->symbols, sort_alpha);
  return EXIT_SUCCESS;
}

void print_sym(void *symbol) {
    t_symbol *sym = (t_symbol *)symbol;

    // Seuls les symboles Undefined ('U', 'w', 'v') avec une valeur 0 n'affichent pas leur adresse
    if (sym->value == 0 && (sym->type == 'U' || sym->type == 'w' || sym->type == 'v')) {
        dprintf(STDOUT_FILENO, "%16s %c %s\n", "", sym->type, sym->name);
    } else {
        dprintf(STDOUT_FILENO, "%016lx %c %s\n", sym->value, sym->type, sym->name);
    }
}

int nm(char *filename, t_options *options, int *_errno) {

  t_nm nm = {0};
  nm.options = options;
  nm._errno = _errno;

  // TODO: parse_elf()
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

  if (S_ISDIR(st.st_mode)) {
    ft_dprintf(STDERR_FILENO, "ft_nm: Warning: '%s' is a directory\n", filename);
    *_errno += 1;
    close(fd);
    return EXIT_SUCCESS;
  }

  // Check if the file is readable
  if (!(st.st_mode & (S_IRUSR | S_IRGRP | S_IROTH))) {
    *_errno += 1;
    close(fd);
    return EXIT_SUCCESS;
  } 

  if ((long unsigned int)st.st_size < sizeof(Elf32_Ehdr)) {
    // ft_dprintf(STDERR_FILENO, "ft_nm: File is too small to be a valid ELF file\n");
    *_errno += 1;
    close(fd);
    return EXIT_SUCCESS;
  }

  nm.file_size = st.st_size;

  nm.file_ptr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (nm.file_ptr == MAP_FAILED) {
    perror("ft_nm: mmap:");
    *_errno += 1;
    close(fd);
    return EXIT_FAILURE;
  }

  Elf64_Ehdr *ehdr = (Elf64_Ehdr *)nm.file_ptr;

  if (ehdr->e_ident[0] != ELFMAG0 || ehdr->e_ident[1] != ELFMAG1 ||
      ehdr->e_ident[2] != ELFMAG2 || ehdr->e_ident[3] != ELFMAG3) {
    ft_dprintf(STDERR_FILENO, "ft_nme: %s: file format not recognized\n", filename);
    *_errno += 1;
    munmap(nm.file_ptr, st.st_size);
    close(fd);
    return EXIT_SUCCESS;
  }

  if (ehdr->e_ident[EI_CLASS] == ELFCLASS64)  {
    nm.elf_class = Elf64;
    if (elf_64(&nm)) {
      *_errno += 1;
      munmap(nm.file_ptr, st.st_size);
      close(fd);
      return EXIT_FAILURE;
    }
  } else if (ehdr->e_ident[EI_CLASS] == ELFCLASS32) {
    nm.elf_class = Elf32;
    if (elf_32(&nm)) {
      *_errno += 1;
      munmap(nm.file_ptr, st.st_size);
      close(fd);
      return EXIT_FAILURE;
    }
  } else {
    ft_dprintf(STDERR_FILENO, "File is not a 64-bit ELF file\n");
    *_errno += 1;
    munmap(nm.file_ptr, st.st_size);
    close(fd);
    return EXIT_SUCCESS;
  }

  if (filter_symbols(&nm)) {
    *_errno += 1;
    munmap(nm.file_ptr, st.st_size);
    close(fd);
    return EXIT_FAILURE;
  }

  sort_symbols(&nm);

  if (nm.options->rev_sort) {
    dlst_foreach_reverse(nm.symbols, print_sym);
  } else {
    dlst_foreach(nm.symbols, print_sym);
  }

  dlst_clear(&nm.symbols, free);
  munmap(nm.file_ptr, st.st_size);
  close(fd);
  return EXIT_SUCCESS;
}

int main(int ac, char **av) {

  t_options options = {0};
  parse_options(ac, av, &options);

  int _errno = 0;

  if (ac == 1) {
    if (nm("a.out", &options, &_errno))
      return _errno;
  }

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
