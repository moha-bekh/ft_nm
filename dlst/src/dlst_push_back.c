#include "dlst_internal.h"

int	dlst_push_back(t_dlst **dlst, void *data)
{
	t_dlst_node	*node;

	if (!dlst)
		return (0);
	node = dlst_node_new();
	if (!node)
		return (0);
	if (!*dlst)
	{
		*dlst = dlst_new();
		if (!*dlst)
			return (free(node), 0);
	}
	if (!(*dlst)->size)
	{
		(*dlst)->head = node;
		(*dlst)->tail = node;
	}
	else
	{
		node->prev = (*dlst)->tail;
		(*dlst)->tail->next = node;
		(*dlst)->tail = node;
	}
	node->mgr = *dlst;
	node->data = data;
	(*dlst)->size++;
	return (1);
}
