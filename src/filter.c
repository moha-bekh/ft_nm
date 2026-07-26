#include "ft_nm.h"

int is_external_symbol(char type)
{
  if (type >= 'A' && type <= 'Z')
      return (1);

  if (type == 'w')
      return (1);

  return (0);
}

int filter_symbols(t_nm *nm) {
  t_dlst_node *node = dlst_head(nm->symbols);
  t_dlst_node *next;

  while (node) {
    t_symbol *symbol = (t_symbol *)dlst_data(node);
    next = dlst_next(node);

    bool keep = true;

    if (nm->options->undefined_only && symbol->type != 'U' && symbol->type != 'w')
      keep = false;

    if (nm->options->global_only && !is_external_symbol(symbol->type))
      keep = false;

    if (keep == false) {
      if (!dlst_del(&nm->symbols, node, free)) {
        ft_dprintf(STDERR_FILENO, "ft_nm: dlst_del failed\n");
        return EXIT_FAILURE;
      }
    }
    node = next;
  }
  return EXIT_SUCCESS;
}
