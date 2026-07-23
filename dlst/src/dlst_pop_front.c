#include "dlst_internal.h"

void	*dlst_pop_front(t_dlst **dlst)
{
	t_dlst_node	*tmp;
	void		*data;

	if (!dlst || !*dlst)
		return (NULL);
	if (!(*dlst)->size || !(*dlst)->head)
		return (free(*dlst), *dlst = NULL, NULL);
	tmp = (*dlst)->head;
	data = tmp->data;
	(*dlst)->size--;
	if ((*dlst)->size)
	{
		(*dlst)->head = tmp->next;
		(*dlst)->head->prev = NULL;
	}
	else
	{
		free(*dlst);
		*dlst = NULL;
	}
	free(tmp);
	return (data);
}
