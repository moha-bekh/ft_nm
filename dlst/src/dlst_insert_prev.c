#include "dlst_internal.h"

int	dlst_insert_prev(t_dlst **dlst, t_dlst_node *node, void *data)
{
	t_dlst_node	*new_node;

	if (!dlst)
		return (0);
	if (!*dlst || !(*dlst)->size || !node)
		return (dlst_push_front(dlst, data));
	if (node->mgr != *dlst)
		return (0);
	new_node = dlst_node_new();
	if (!new_node)
		return (0);
	new_node->next = node;
	new_node->prev = node->prev;
	if (node->prev)
		node->prev->next = new_node;
	else
		(*dlst)->head = new_node;
	node->prev = new_node;
	new_node->mgr = *dlst;
	new_node->data = data;
	(*dlst)->size++;
	return (1);
}
