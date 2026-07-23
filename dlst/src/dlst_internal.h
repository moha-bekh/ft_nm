#ifndef DLST_INTERNAL_H
# define DLST_INTERNAL_H

# include "dlst.h"
# include <stdlib.h>

struct s_dlst_node {
	t_dlst_node	*next;
	t_dlst_node	*prev;
	t_dlst		*mgr;
	void		*data;
};

struct s_dlst {
	t_dlst_node	*head;
	t_dlst_node	*tail;
	size_t		size;
};

t_dlst_node	*dlst_node_new(void);

#endif
