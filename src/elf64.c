#include "ft_nm.h"

static inline bool is_safe_ptr(t_nm *nm, void *ptr, size_t size) {
    if (!ptr)
        return false;
    uintptr_t start = (uintptr_t)nm->file_ptr;
    uintptr_t end = start + nm->file_size;
    uintptr_t check_start = (uintptr_t)ptr;
    uintptr_t check_end = check_start + size;

    return (check_start >= start && check_end <= end && check_end >= check_start);
}

char get_64_symbol_type(Elf64_Sym *sym, Elf64_Shdr *shdr, uint16_t e_shnum)
{
    unsigned char bind = ELF64_ST_BIND(sym->st_info);
    unsigned char type = ELF64_ST_TYPE(sym->st_info);
    char          c;

    if (bind == STB_WEAK)
    {
        c = (type == STT_OBJECT) ? 'V' : 'W';
        if (sym->st_shndx == SHN_UNDEF)
            c = (type == STT_OBJECT) ? 'v' : 'w';
        return (c);
    }

    if (sym->st_shndx == SHN_UNDEF)
        return ('U');
    if (sym->st_shndx == SHN_ABS)
        c = 'A';
    else if (sym->st_shndx == SHN_COMMON)
        c = 'C';
    else if (sym->st_shndx < e_shnum)
    {
        Elf64_Word  sh_type  = shdr[sym->st_shndx].sh_type;
        Elf64_Xword sh_flags = shdr[sym->st_shndx].sh_flags;

        if (sh_type == SHT_NOBITS && (sh_flags & SHF_ALLOC))
            c = 'B';
        else if (sh_type == SHT_PROGBITS && (sh_flags & SHF_EXECINSTR))
            c = 'T';
        else if (sh_flags & SHF_WRITE)
            c = 'D';
        else if (sh_flags & SHF_ALLOC)
            c = 'R';
        else
            c = 'N';
    }
    else
        return ('?');

    if (bind == STB_LOCAL && c >= 'A' && c <= 'Z')
        c += 32;

    return (c);
}

int elf_64(t_nm *nm, char *filename) {
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)nm->file_ptr;

    // 1. Vérification de l'Ehdr 64-bit
    if (!is_safe_ptr(nm, ehdr, sizeof(Elf64_Ehdr))) {
        ft_dprintf(STDERR_FILENO, "ft_nm: %s: file format not recognized\n", filename);
        return NO_EXIT_FAILURE;
    }

    // 2. Controler le tableau de Section Headers
    Elf64_Shdr *shdr = (Elf64_Shdr *)((char *)nm->file_ptr + ehdr->e_shoff);
    if (!is_safe_ptr(nm, shdr, (size_t)ehdr->e_shnum * sizeof(Elf64_Shdr))) {
        ft_dprintf(STDERR_FILENO, "ft_nm: %s: file format not recognized\n", filename);
        return NO_EXIT_FAILURE;
    }

    // 3. Contrôler shstrtab
    char *shstrtab = NULL;
    if (ehdr->e_shstrndx < ehdr->e_shnum) {
        Elf64_Shdr *shstrtab_hdr = &shdr[ehdr->e_shstrndx];
        shstrtab = (char *)nm->file_ptr + shstrtab_hdr->sh_offset;
        if (!is_safe_ptr(nm, shstrtab, shstrtab_hdr->sh_size))
            shstrtab = NULL;
    }

    for (int i = 0; i < ehdr->e_shnum; i++) {
        if (shdr[i].sh_type != SHT_SYMTAB)
            continue;

        if (shdr[i].sh_entsize == 0)
            continue;

        Elf64_Sym *symtab = (Elf64_Sym *)((char *)nm->file_ptr + shdr[i].sh_offset);
        if (!is_safe_ptr(nm, symtab, shdr[i].sh_size))
            continue;

        int sym_count = shdr[i].sh_size / shdr[i].sh_entsize;
        unsigned int section_link = shdr[i].sh_link;

        if (section_link >= ehdr->e_shnum)
            continue;

        char *strtab = (char *)nm->file_ptr + shdr[section_link].sh_offset;
        if (!is_safe_ptr(nm, strtab, shdr[section_link].sh_size))
            continue;

        for (int j = 1; j < sym_count; j++) {
            if (!is_safe_ptr(nm, &symtab[j], sizeof(Elf64_Sym)))
                break;

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
            symbol->type = get_64_symbol_type(&symtab[j], shdr, ehdr->e_shnum);

            if (st_type == STT_SECTION) {
                if (shstrtab && symtab[j].st_shndx < ehdr->e_shnum &&
                    is_safe_ptr(nm, shstrtab + shdr[symtab[j].st_shndx].sh_name, 1))
                    symbol->name = shstrtab + shdr[symtab[j].st_shndx].sh_name;
                else
                    symbol->name = "";
            } else {
                if (is_safe_ptr(nm, strtab + symtab[j].st_name, 1))
                    symbol->name = strtab + symtab[j].st_name;
                else
                    symbol->name = "";
            }

            if (!dlst_push_back(&nm->symbols, symbol)) {
                free(symbol);
                ft_dprintf(STDERR_FILENO, "ft_nm: dlst_push_back failed\n");
                return EXIT_FAILURE;
            }
        }
    }

    if (!dlst_size(nm->symbols)) {
        ft_dprintf(STDERR_FILENO, "ft_nm: %s: no symbols\n", filename);
        return EXIT_SUCCESS;
    }
    return EXIT_SUCCESS;
}