#include "dlst_internal.h"

static void	dlst_swap_data(t_dlst_node *a, t_dlst_node *b)
{
	void	*tmp;

	tmp = a->data;
	a->data = b->data;
	b->data = tmp;
}

void	dlst_sort(t_dlst *dlst, int (*cmp)(void *, void *))
{
	t_dlst_node	*current;
	t_dlst_node	*next;

	if (!dlst || dlst->size < 2 || !cmp)
		return ;
	current = dlst->head;
	while (current)
	{
		next = current->next;
		while (next)
		{
			if (cmp(current->data, next->data) > 0)
				dlst_swap_data(current, next);
			next = next->next;
		}
		current = current->next;
	}
}
