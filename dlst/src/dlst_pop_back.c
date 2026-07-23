#include "dlst_internal.h"

void	*dlst_pop_back(t_dlst **dlst)
{
	t_dlst_node	*tmp;
	void		*data;

	if (!dlst || !*dlst)
		return (NULL);
	if (!(*dlst)->size || !(*dlst)->tail)
		return (free(*dlst), *dlst = NULL, NULL);
	tmp = (*dlst)->tail;
	data = tmp->data;
	(*dlst)->size--;
	if ((*dlst)->size)
	{
		(*dlst)->tail = tmp->prev;
		(*dlst)->tail->next = NULL;
	}
	else
	{
		free(*dlst);
		*dlst = NULL;
	}
	free(tmp);
	return (data);
}
