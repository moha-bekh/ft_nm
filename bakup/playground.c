#include "ft_nm.h"

#include "print.c"

char get_64_symbol_type(Elf64_Sym *sym, Elf64_Shdr *shdr) {
  if (sym->st_shndx == SHN_UNDEF)
    return 'U';
  else if (sym->st_shndx == SHN_ABS)
    return 'A';
  else if (sym->st_shndx == SHN_COMMON)
    return 'C';
  else if (shdr[sym->st_shndx].sh_type == SHT_NOBITS &&
           shdr[sym->st_shndx].sh_flags & SHF_ALLOC &&
           shdr[sym->st_shndx].sh_flags & SHF_WRITE)
    return 'B';
  else if (shdr[sym->st_shndx].sh_type == SHT_PROGBITS &&
           shdr[sym->st_shndx].sh_flags & SHF_ALLOC &&
           shdr[sym->st_shndx].sh_flags & SHF_EXECINSTR)
    return 'T';
  else if (shdr[sym->st_shndx].sh_type == SHT_PROGBITS &&
           shdr[sym->st_shndx].sh_flags & SHF_ALLOC &&
           !(shdr[sym->st_shndx].sh_flags & SHF_EXECINSTR))
    return 'R';
  else if (shdr[sym->st_shndx].sh_type == SHT_PROGBITS &&
           !(shdr[sym->st_shndx].sh_flags & SHF_ALLOC))
    return 'D';
  else
    return '?';
}

int main(int ac, char **av) {

  if (ac < 2) {
    fprintf(stderr, "Usage: %s <elf-file>\n", av[0]);
    return (1);
  }

  t_nm nm = {0};

  nm.fd = open(av[1], O_RDONLY);
  fstat(nm.fd, &nm.stat);
  nm.file_ptr = mmap(NULL, nm.stat.st_size, PROT_READ, MAP_PRIVATE, nm.fd, 0);

  nm.elf.ehdr = (Elf64_Ehdr *)nm.file_ptr;
  nm.elf.shdr = (Elf64_Shdr *)((char *)nm.file_ptr + nm.elf.ehdr->e_shoff);
  nm.elf.shstrtab_hdr = &nm.elf.shdr[nm.elf.ehdr->e_shstrndx];
  nm.elf.shstrtab = (char *)nm.file_ptr + nm.elf.shstrtab_hdr->sh_offset;

  // print_ehdr(nm.elf.ehdr);

  for (int i = 0; i < nm.elf.ehdr->e_shnum; i++) {

    if (nm.elf.shdr[i].sh_type != SHT_SYMTAB)
      continue;

    Elf64_Sym *symtab =
        (Elf64_Sym *)((char *)nm.file_ptr + nm.elf.shdr[i].sh_offset);
    int sym_count = nm.elf.shdr[i].sh_size / nm.elf.shdr[i].sh_entsize;
    unsigned int section_link = nm.elf.shdr[i].sh_link;
    char *strtab = (char *)nm.file_ptr + nm.elf.shdr[section_link].sh_offset;

    for (int j = 0; j < sym_count; j++) {
      char type = get_64_symbol_type(&symtab[j], nm.elf.shdr);
      long long value = symtab[j].st_value;
      if (value == 0) {
        printf("%16s %c %s\n", "", type, strtab + symtab[j].st_name);
      } else {
        printf("%.016lx %c %s\n", symtab[j].st_value, type,
               strtab + symtab[j].st_name);
      }
    }
  }

  munmap(nm.file_ptr, nm.stat.st_size);
  close(nm.fd);
  return 0;
}

/*
open OK
↓
fstat OK
↓
mmap OK
↓
validate ELF
↓
parse headers
↓
find symtab/dynsym
↓
extract symbols
↓
compute nm type
↓
filter
↓
sort
↓
display
↓
cleanup
*/
