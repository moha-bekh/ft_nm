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

    if (sym->value == 0 && (sym->type == 'U' || sym->type == 'w' || sym->type == 'v')) {
        // 16 espaces fixes + type + nom
        ft_dprintf(STDOUT_FILENO, "                 %c %s\n", sym->type, sym->name);
    } else {
        // 1. Zéros de rembourrage jusqu'à 16 caractères
        print_zeros(STDOUT_FILENO, sym->value, 16);
        // 2. Valeur en hexa 64 bits + type + nom
        ft_dprintf(STDOUT_FILENO, "%lx %c %s\n", sym->value, sym->type, sym->name);
    }
}

void print_32_sym(void *symbol) {
    t_symbol *sym = (t_symbol *)symbol;

    if (sym->value == 0 && (sym->type == 'U' || sym->type == 'w' || sym->type == 'v')) {
        // 8 espaces fixes + type + nom
        ft_dprintf(STDOUT_FILENO, "         %c %s\n", sym->type, sym->name);
    } else {
        // 1. Zéros de rembourrage jusqu'à 8 caractères
        print_zeros(STDOUT_FILENO, sym->value, 8);
        // 2. Valeur en hexa 32 bits + type + nom
        ft_dprintf(STDOUT_FILENO, "%lx %c %s\n", sym->value, sym->type, sym->name);
    }
}

// void print_64_sym(void *symbol) {
//     t_symbol *sym = (t_symbol *)symbol;
//
//     // Seuls les symboles Undefined ('U', 'w', 'v') avec une valeur 0 n'affichent pas leur adresse
//     if (sym->value == 0 && (sym->type == 'U' || sym->type == 'w' || sym->type == 'v')) {
//         dprintf(STDOUT_FILENO, "%16s %c %s\n", "", sym->type, sym->name);
//     } else {
//         dprintf(STDOUT_FILENO, "%016lx %c %s\n", sym->value, sym->type, sym->name);
//     }
// }
//
// void print_32_sym(void *symbol) {
//     t_symbol *sym = (t_symbol *)symbol;
//
//     // Seuls les symboles Undefined ('U', 'w', 'v') avec une valeur 0 n'affichent pas leur adresse
//     if (sym->value == 0 && (sym->type == 'U' || sym->type == 'w' || sym->type == 'v')) {
//         dprintf(STDOUT_FILENO, "%8s %c %s\n", "", sym->type, sym->name);
//     } else {
//         dprintf(STDOUT_FILENO, "%08lx %c %s\n", sym->value, sym->type, sym->name);
//     }
// }
//
