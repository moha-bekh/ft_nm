#include "dlst_internal.h"

void	dlst_clear(t_dlst **dlst, void (*del)(void *))
{
	void	*data;

	if (!dlst)
		return ;
	while (*dlst)
	{
		data = dlst_pop_back(dlst);
		if (del && data)
			del(data);
	}
}
