#include "ft_nm.h"

// Calcule la longueur en hexa d'une valeur et imprime les zéros manquants
static void print_zeros(int fd, uint64_t val, int target_width)
{
    int len = 0;
    uint64_t tmp = val;

    if (tmp == 0)
        len = 1;
    while (tmp > 0) {
        len++;
        tmp /= 16;
    }

    // Imprime le nombre exact de zéros pour atteindre 8 ou 16 caractères
    while (len < target_width) {
        ft_dprintf(fd, "0");
        len++;
    }
}

void print_64_sym(void *symbol) {
    t_symbol *sym = (t_symbol *)symbol;

    // 'U' a TOUJOURS des espaces. 'w' et 'v' ont des espaces uniquement si leur valeur est 0.
    if (sym->type == 'U' || (sym->value == 0 && (sym->type == 'w' || sym->type == 'v'))) {
        ft_dprintf(STDOUT_FILENO, "                 %c %s\n", sym->type, sym->name);
    } else {
        print_zeros(STDOUT_FILENO, sym->value, 16);
        ft_dprintf(STDOUT_FILENO, "%lx %c %s\n", sym->value, sym->type, sym->name);
    }
}

void print_32_sym(void *symbol) {
    t_symbol *sym = (t_symbol *)symbol;

    if (sym->type == 'U' || (sym->value == 0 && (sym->type == 'w' || sym->type == 'v'))) {
        ft_dprintf(STDOUT_FILENO, "         %c %s\n", sym->type, sym->name);
    } else {
        print_zeros(STDOUT_FILENO, sym->value, 8);
        ft_dprintf(STDOUT_FILENO, "%lx %c %s\n", sym->value, sym->type, sym->name);
    }
}