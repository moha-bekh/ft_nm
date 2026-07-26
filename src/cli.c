#include "ft_nm.h"

void print_debug_options(t_options *options) {
  ft_dprintf(STDERR_FILENO, "a[%d] g[%d] u[%d] r[%d] p[%d]\n",
      options->all,
      options->global_only,
      options->undefined_only,
      options->rev_sort,
      options->no_sort
  );
}

void parse_options(int ac,
          char **av, 
          t_options *options
){
  for (int i = 1; i < ac; i++) {
    if (!ft_strcmp(av[i], "--")) {
      return ;
    }
    else if (av[i][0] == '-' && av[i][1]) {
      for (int j = 1; av[i][j]; j++) {
        switch (av[i][j]) {
          case 'a':
            options->all = true;
            break;
          case 'g':
            options->global_only = true;
            break;
          case 'u':
            options->undefined_only = true;
            break;
          case 'r':
            options->rev_sort = true;
            break;
          case 'p':
            options->no_sort = true;
            break;
          default:
            ft_dprintf(STDERR_FILENO,
                "ft_nm: invalid option -- '%c'\n"
                "Usage: ft_nm [option(s)] [file(s)]\n"
                " List symbols in [file(s)] (a.out by default).\n"
                " The options are:\n"
                "  -a  Display debugger-only symbols\n"
                "  -g  Display only external symbols\n"
                "  -p  Do not sort the symbols\n"
                "  -r  Reverse the order of the sort\n"
                "  -u  Display only undefined symbols\n",
                av[i][j]
            );
            exit(EXIT_FAILURE);
        }
      }
    }
  }

  // print_debug_options(options);
};
