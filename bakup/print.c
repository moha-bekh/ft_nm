
void print_ehdr(Elf64_Ehdr *ehdr) {

  printf("ELF HEADER --------------------------------------------------\n\n");
  printf("ident -> ");
  for (int i = 0; i < EI_NIDENT; i++) {
    printf("%02x ", ehdr->e_ident[i]);
  }
  printf("\n");
  printf("EI_CLASS | binary architecture -> %d\n", ehdr->e_ident[EI_CLASS]);
  printf("EI_DATA | codage or endieness -> %d\n", ehdr->e_ident[EI_DATA]);
  printf("type | elf type-> %d\n", ehdr->e_type);
  printf("machine | elf machine -> %d\n", ehdr->e_machine);
  printf("shoff | section header offset -> %lu\n", ehdr->e_shoff);
  printf("shnum | section header count -> %d\n", ehdr->e_shnum);
  printf("shentsize | section header size -> %d\n", ehdr->e_shentsize);
  printf("shstrndx | section header string index -> %d\n", ehdr->e_shstrndx);

  printf("\n");
}

void print_shdr(Elf64_Shdr *shdr, char *shstrtab, int shnum, void *file_ptr) {
  printf(
      "SECTION HEADER --------------------------------------------------\n\n");

  int strtab_index = -1;

  for (int i = 0; i < shnum; i++) {

    if (shdr[i].sh_type != SHT_SYMTAB && shdr[i].sh_type != SHT_DYNSYM)
      continue;

    printf("index -> %d\n", i);
    printf("name -> %s\n", shstrtab + shdr[i].sh_name);
    printf("symbol count -> %lu\n", shdr[i].sh_size / shdr[i].sh_entsize);

    printf("link strtab index -> %d\n", shdr[i].sh_link);
    printf("link strtab name -> %s\n",
           shstrtab + shdr[shdr[i].sh_link].sh_name);

    printf("string table size -> %lu\n", shdr[i].sh_size);
    printf("link strtab index -> %d\n", shdr[i].sh_link);

    printf("link strtab name -> %s\n",
           shstrtab + shdr[shdr[i].sh_link].sh_name);
    printf("\n");

    Elf64_Sym *symtab =
        (Elf64_Sym *)((char *)file_ptr + shdr[i].sh_offset); // symbol table
    int sym_count = shdr[i].sh_size / shdr[i].sh_entsize;    // symbol count
    char *strtab =
        (char *)file_ptr + shdr[shdr[i].sh_link].sh_offset; // string table

    for (int j = 0; j < sym_count; j++) {
      // printf("symbol[%d] -> %s\n", j, strtab + symtab[j].st_name);
      printf("symname -> %s\n", strtab + symtab[j].st_name);
      Elf64_Shdr *strtab_hdr = &shdr[shdr[i].sh_link];
      printf("symtab_size -> %lu\n", shdr[i].sh_size);
      printf("strtab_size -> %lu\n", strtab_hdr->sh_size);
      // printf("name -> %s\n", strtab + symtab[j].st_name);
      // printf("value -> 0x%lx\n", symtab[j].st_value);
      // printf("size -> %lu\n", symtab[j].st_size);
      // printf("shndx -> %d\n", symtab[j].st_shndx);
      // printf("ST_BIND -> %d\n", ELF64_ST_BIND(symtab[j].st_info));
      // printf("ST_TYPE -> %d\n", ELF64_ST_TYPE(symtab[j].st_info));
      // printf("ST_VISIBILITY -> %d\n",
      // ELF64_ST_VISIBILITY(symtab[j].st_other));
      printf("\n");
    }

    printf("\n");
  }

  printf("\n");
}
