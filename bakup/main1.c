#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

void _print_stat(struct stat *st);
void _print_ehdr(Elf64_Ehdr *ehdr);

void print_elf(void *ptr) {

  // elf header
  Elf64_Ehdr *ehdr = (Elf64_Ehdr *)ptr;

  printf("ELF HEADER\n");
  printf("  e_ident:       ");
  for (int i = 0; i < EI_NIDENT; i++)
    printf("%02x ", ehdr->e_ident[i]);
  printf("\n");
  printf("  e_type:        %u\n", ehdr->e_type);
  printf("  e_machine:     %u\n", ehdr->e_machine);
  printf("  e_version:     %u\n", ehdr->e_version);
  printf("  e_entry:       0x%lx\n", (unsigned long)ehdr->e_entry);
  printf("  e_phoff:       %lu\n", (unsigned long)ehdr->e_phoff);
  printf("  e_shoff:       %lu\n", (unsigned long)ehdr->e_shoff); // section header offset
  printf("  e_flags:       %u\n", ehdr->e_flags);
  printf("  e_ehsize:      %u\n", ehdr->e_ehsize);
  printf("  e_phentsize:   %u\n", ehdr->e_phentsize);
  printf("  e_phnum:       %u\n", ehdr->e_phnum);
  printf("  e_shentsize:   %u\n", ehdr->e_shentsize);
  printf("  e_shnum:       %u\n", ehdr->e_shnum); // section header count
  printf("  e_shstrndx:    %u\n", ehdr->e_shstrndx); // section header string table index

  // section headers
  Elf64_Shdr *shdr = (Elf64_Shdr *)((char *)ptr + ehdr->e_shoff); // section hader table

  // section header string table
  Elf64_Shdr shstrtab_hdr = shdr[ehdr->e_shstrndx]; // section header for the section header string table
  char *shstrtab = (char *)ptr + shstrtab_hdr.sh_offset; // pointer to the section header string table

  printf("\nSECTION HEADERS\n");
  for (int i = 0; i < ehdr->e_shnum; i++) {
    printf("[%d] %s\n", i, shstrtab + shdr[i].sh_name); // section name
    printf("  sh_name:       %u\n", shdr[i].sh_name);
    printf("  sh_type:       %u\n", shdr[i].sh_type);
    printf("  sh_flags:      0x%lx\n", (unsigned long)shdr[i].sh_flags);
    printf("  sh_addr:       0x%lx\n", (unsigned long)shdr[i].sh_addr);
    printf("  sh_offset:     %lu\n", (unsigned long)shdr[i].sh_offset);
    printf("  sh_size:       %lu\n", (unsigned long)shdr[i].sh_size);
    printf("  sh_link:       %u\n", shdr[i].sh_link);
    printf("  sh_info:       %u\n", shdr[i].sh_info);
    printf("  sh_addralign:  %lu\n", (unsigned long)shdr[i].sh_addralign);
    printf("  sh_entsize:    %lu\n", (unsigned long)shdr[i].sh_entsize);
  }

  printf("\nSYMBOL TABLES\n");
  for (int i = 0; i < ehdr->e_shnum; i++) {
    if (shdr[i].sh_type != SHT_SYMTAB && shdr[i].sh_type != SHT_DYNSYM)
      continue;

    Elf64_Sym *symtab = (Elf64_Sym *)((char *)ptr + shdr[i].sh_offset);
    int sym_count = shdr[i].sh_size / shdr[i].sh_entsize;
    char *strtab = (char *)ptr + shdr[shdr[i].sh_link].sh_offset;

    printf("\n[%d] %s (%d symbols, strings in section [%u] %s)\n", i, shstrtab + shdr[i].sh_name, sym_count, shdr[i].sh_link, shstrtab + shdr[shdr[i].sh_link].sh_name);

    for (int j = 0; j < sym_count; j++) {
      printf("  symbol[%d] %s\n", j, strtab + symtab[j].st_name);
      printf("    st_name:     %u\n", symtab[j].st_name);
      printf("    st_info:     %u bind=%u type=%u\n", symtab[j].st_info, ELF64_ST_BIND(symtab[j].st_info), ELF64_ST_TYPE(symtab[j].st_info));
      printf("    st_other:    %u visibility=%u\n", symtab[j].st_other, ELF64_ST_VISIBILITY(symtab[j].st_other));
      printf("    st_shndx:    %u\n", symtab[j].st_shndx);
      printf("    st_value:    0x%lx\n", (unsigned long)symtab[j].st_value);
      printf("    st_size:     %lu\n", (unsigned long)symtab[j].st_size);
    }
  }
}

/*

  open/mmap
  ↓
  parse ELF
  ↓
  extraire symboles dans un tableau
  ↓
  filtrer selon options
  ↓
  trier selon options
  ↓
  print
  ↓
  cleanup

*/
typedef struct s_symbol
{
    char            *name;
    unsigned long   value;
    char            type;
    unsigned char   bind;
    unsigned char   sym_type;
    unsigned short  shndx;
}   t_symbol;

int main(int ac, char **av) {

  if (ac < 2) {
    fprintf(stderr, "Usage: %s <elf-file>\n", av[0]);
    return (1);
  }

  int fd = open(av[1], O_RDONLY);
  if (fd < 0) {
    perror("open");
    return (1);
  }

  struct stat st;
  int status = fstat(fd, &st);
  if (status < 0) {
    perror("fstat");
    return (1);
  }

  // _print_stat(&st);

  if ((long unsigned int)st.st_size < sizeof(Elf64_Ehdr)) {
    fprintf(stderr, "File is too small to be a valid ELF file\n");
    close(fd);
    return (1);
  }

  void *ptr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (ptr == MAP_FAILED) {
    perror("mmap");
    return (1);
  }


  Elf64_Ehdr *ehdr = (Elf64_Ehdr *)ptr;

  if (ehdr->e_ident[0] != ELFMAG0 || ehdr->e_ident[1] != ELFMAG1 ||
      ehdr->e_ident[2] != ELFMAG2 || ehdr->e_ident[3] != ELFMAG3) {
    fprintf(stderr, "File is not a valid ELF file\n");
    munmap(ptr, st.st_size);
    close(fd);
    return (1);
  }

  if (ehdr->e_ident[EI_CLASS] != ELFCLASS64) {
    fprintf(stderr, "File is not a 64-bit ELF file\n");
    munmap(ptr, st.st_size);
    close(fd);
    return (1);
  }

  print_elf(ptr);

  munmap(ptr, st.st_size);

  close(fd);
  return (0);
}
