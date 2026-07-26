#include "ft_nm.h"

char get_32_symbol_type(Elf32_Sym *sym, Elf32_Shdr *shdr)
{
    unsigned char bind = ELF32_ST_BIND(sym->st_info);
    unsigned char type = ELF32_ST_TYPE(sym->st_info);
    char          c;

    // Weak Symbols
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
        Elf32_Word sh_type  = shdr[sym->st_shndx].sh_type;
        Elf32_Word sh_flags = shdr[sym->st_shndx].sh_flags;

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

int elf_32(t_nm *nm) {
  Elf32_Ehdr *ehdr = (Elf32_Ehdr *)nm->file_ptr;
  Elf32_Shdr *shdr = (Elf32_Shdr *)((char *)nm->file_ptr + ehdr->e_shoff);
  Elf32_Shdr *shstrtab_hdr = &shdr[ehdr->e_shstrndx];
  char *shstrtab = (char *)nm->file_ptr + shstrtab_hdr->sh_offset;

  for (int i = 0; i < ehdr->e_shnum; i++) {
    if (shdr[i].sh_type != SHT_SYMTAB)
      continue;

    Elf32_Sym *symtab = (Elf32_Sym *)((char *)nm->file_ptr + shdr[i].sh_offset);
    int sym_count = shdr[i].sh_size / shdr[i].sh_entsize;
    unsigned int section_link = shdr[i].sh_link;
    char *strtab = (char *)nm->file_ptr + shdr[section_link].sh_offset;

    for (int j = 1; j < sym_count; j++) {

      unsigned char st_type = ELF32_ST_TYPE(symtab[j].st_info);
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
      symbol->type = get_32_symbol_type((Elf32_Sym *)&symtab[j], (Elf32_Shdr *)shdr);
      symbol->name = strtab + symtab[j].st_name;

      if (!dlst_push_back(&nm->symbols, symbol)) {
        ft_dprintf(STDERR_FILENO, "ft_nm: dlst_push_back failed\n");
        return EXIT_FAILURE;
      }
    }
  }
  return EXIT_SUCCESS;
}
