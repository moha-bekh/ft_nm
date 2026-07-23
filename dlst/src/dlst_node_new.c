#include "dlst_internal.h"

t_dlst_node	*dlst_node_new(void)
{
	t_dlst_node	*node;

	node = malloc(sizeof(t_dlst_node));
	if (!node)
		return (NULL);
	*node = (t_dlst_node){0};
	return (node);
}
