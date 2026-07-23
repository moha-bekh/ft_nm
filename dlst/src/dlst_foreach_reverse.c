#include "dlst_internal.h"

void	dlst_foreach_reverse(t_dlst *dlst, void (*f)(void *))
{
	t_dlst_node	*tmp;

	if (!dlst || !f)
		return ;
	tmp = dlst->tail;
	while (tmp)
	{
		f(tmp->data);
		tmp = tmp->prev;
	}
}
