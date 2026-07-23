#include "dlst_internal.h"

void	dlst_foreach(t_dlst *dlst, void (*f)(void *))
{
	t_dlst_node	*tmp;

	if (!dlst || !f)
		return ;
	tmp = dlst->head;
	while (tmp)
	{
		f(tmp->data);
		tmp = tmp->next;
	}
}
