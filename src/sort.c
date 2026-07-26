#include "ft_nm.h"

static int sort_alpha(void *a, void *b) {
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

