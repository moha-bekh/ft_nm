#include "dlst_internal.h"

int	dlst_insert_next(t_dlst **dlst, t_dlst_node *node, void *data)
{
	t_dlst_node	*new_node;

	if (!dlst)
		return (0);
	if (!*dlst || !(*dlst)->size || !node)
		return (dlst_push_back(dlst, data));
	if (node->mgr != *dlst)
		return (0);
	new_node = dlst_node_new();
	if (!new_node)
		return (0);
	new_node->prev = node;
	new_node->next = node->next;
	if (node->next)
		node->next->prev = new_node;
	else
		(*dlst)->tail = new_node;
	node->next = new_node;
	new_node->data = data;
	new_node->mgr = *dlst;
	(*dlst)->size++;
	return (1);
}
