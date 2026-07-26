#include "ft_nm.h"

int check_elf(t_nm *nm, char *filename) {

  Elf64_Ehdr *ehdr = (Elf64_Ehdr *)nm->file_ptr;

  if (ehdr->e_ident[0] != ELFMAG0 || ehdr->e_ident[1] != ELFMAG1 ||
      ehdr->e_ident[2] != ELFMAG2 || ehdr->e_ident[3] != ELFMAG3) {
    ft_dprintf(STDERR_FILENO, "ft_nm: %s: file format not recognized\n", filename);
    return EXIT_FAILURE;
  }

  if (ehdr->e_ident[EI_CLASS] != ELFCLASS32 && ehdr->e_ident[EI_CLASS] != ELFCLASS64) {
    ft_dprintf(STDERR_FILENO, "ft_nm: %s: file format not recognized\n", filename);
    return EXIT_FAILURE;
  }

  if (ehdr->e_ident[EI_CLASS] == ELFCLASS32) {
    nm->elf_class = Elf32;
    nm->parse = elf_32;
    nm->print = print_32_sym;
  } else if (ehdr->e_ident[EI_CLASS] == ELFCLASS64) {
    nm->elf_class = Elf64;
    nm->parse = elf_64;
    nm->print = print_64_sym;
  }

  if (ehdr->e_type != ET_EXEC && ehdr->e_type != ET_REL && ehdr->e_type != ET_DYN) {
    ft_dprintf(STDERR_FILENO, "ft_nm: %s: file format not recognized\n", filename);
    return EXIT_FAILURE;
  }

  if (ehdr->e_machine != EM_X86_64 && ehdr->e_machine != EM_386) {
    ft_dprintf(STDERR_FILENO, "ft_nm: %s: file format not recognized\n", filename);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int map_elf(t_nm *nm, char *filename) {

  int fd = open(filename, O_RDONLY);
  if (fd < 0) {
    ft_dprintf(STDERR_FILENO, "ft_nm: '%s': No such file\n", filename);
    return NO_EXIT_FAILURE;
  }

  struct stat st;
  if (fstat(fd, &st) < 0) {
    ft_dprintf(STDERR_FILENO, "ft_nm: fstat: failed '%s'\n", filename);
    close(fd);
    return NO_EXIT_FAILURE;
  }

  if (S_ISDIR(st.st_mode)) {
    ft_dprintf(STDERR_FILENO, "ft_nm: Warning: '%s' is a directory\n", filename);
    close(fd);
    return NO_EXIT_FAILURE;
  }

  // Check if the file is readable
  if (!(st.st_mode & (S_IRUSR | S_IRGRP | S_IROTH))) {
    close(fd);
    return NO_EXIT_FAILURE;
  } 

  if ((long unsigned int)st.st_size < sizeof(Elf32_Ehdr)) {
    // ft_dprintf(STDERR_FILENO, "ft_nm: File is too small to be a valid ELF file\n");
    close(fd);
    return NO_EXIT_FAILURE;
  }

  nm->file_size = st.st_size;

  nm->file_ptr = mmap(NULL, nm->file_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (nm->file_ptr == MAP_FAILED) {
    perror("ft_nm: mmap:");
    close(fd);
    return NO_EXIT_FAILURE;
  }

  close(fd);

  return EXIT_SUCCESS;
}

int parse_elf(t_nm *nm, char *filename) {

  if (check_elf(nm, filename)) {
    return NO_EXIT_FAILURE;
  }

  if (nm->elf_class == Elf64) {
    if (elf_64(nm)) {
      return NO_EXIT_FAILURE;
    }
  } else if (nm->elf_class == Elf32) {
    if (elf_32(nm)) {
      return NO_EXIT_FAILURE;
    }
  } else {
    ft_dprintf(STDERR_FILENO, "File is not a valid ELF file\n");
    return NO_EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
