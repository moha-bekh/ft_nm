#include "dlst_internal.h"

size_t	dlst_size(const t_dlst *dlst)
{
	if (!dlst)
		return (0);
	return (dlst->size);
}

t_dlst_node	*dlst_head(const t_dlst *dlst)
{
	if (!dlst)
		return (NULL);
	return (dlst->head);
}

t_dlst_node	*dlst_tail(const t_dlst *dlst)
{
	if (!dlst)
		return (NULL);
	return (dlst->tail);
}

t_dlst_node	*dlst_next(const t_dlst_node *node)
{
	if (!node)
		return (NULL);
	return (node->next);
}

t_dlst_node	*dlst_prev(const t_dlst_node *node)
{
	if (!node)
		return (NULL);
	return (node->prev);
}

void	*dlst_data(const t_dlst_node *node)
{
	if (!node)
		return (NULL);
	return (node->data);
}
