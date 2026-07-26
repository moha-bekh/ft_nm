#include "ft_nm.h"

void print_64_sym(void *symbol) {
    t_symbol *sym = (t_symbol *)symbol;

    // Seuls les symboles Undefined ('U', 'w', 'v') avec une valeur 0 n'affichent pas leur adresse
    if (sym->value == 0 && (sym->type == 'U' || sym->type == 'w' || sym->type == 'v')) {
        dprintf(STDOUT_FILENO, "%16s %c %s\n", "", sym->type, sym->name);
    } else {
        dprintf(STDOUT_FILENO, "%016lx %c %s\n", sym->value, sym->type, sym->name);
    }
}

void print_32_sym(void *symbol) {
    t_symbol *sym = (t_symbol *)symbol;

    // Seuls les symboles Undefined ('U', 'w', 'v') avec une valeur 0 n'affichent pas leur adresse
    if (sym->value == 0 && (sym->type == 'U' || sym->type == 'w' || sym->type == 'v')) {
        dprintf(STDOUT_FILENO, "%8s %c %s\n", "", sym->type, sym->name);
    } else {
        dprintf(STDOUT_FILENO, "%08lx %c %s\n", sym->value, sym->type, sym->name);
    }
}

