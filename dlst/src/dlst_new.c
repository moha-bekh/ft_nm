#include "dlst_internal.h"

t_dlst	*dlst_new(void)
{
	t_dlst	*dlst;

	dlst = malloc(sizeof(t_dlst));
	if (!dlst)
		return (NULL);
	*dlst = (t_dlst){0};
	return (dlst);
}
