#ifndef DLST_H
# define DLST_H

# include <stddef.h>

typedef struct s_dlst		t_dlst;
typedef struct s_dlst_node	t_dlst_node;

t_dlst		*dlst_new(void);
void		dlst_clear(t_dlst **dlst, void (*del)(void *));

int			dlst_push_back(t_dlst **dlst, void *data);
int			dlst_push_front(t_dlst **dlst, void *data);

int			dlst_insert_next(t_dlst **dlst, t_dlst_node *node, void *data);
int			dlst_insert_prev(t_dlst **dlst, t_dlst_node *node, void *data);

void		*dlst_pop_front(t_dlst **dlst);
void		*dlst_pop_back(t_dlst **dlst);
int			dlst_del(t_dlst **dlst, t_dlst_node *node, void (*del)(void *));

void		dlst_foreach(t_dlst *dlst, void (*f)(void *));
void		dlst_foreach_reverse(t_dlst *dlst, void (*f)(void *));
void		dlst_sort(t_dlst *dlst, int (*cmp)(void *, void *));

size_t		dlst_size(const t_dlst *dlst);
t_dlst_node	*dlst_head(const t_dlst *dlst);
t_dlst_node	*dlst_tail(const t_dlst *dlst);
t_dlst_node	*dlst_next(const t_dlst_node *node);
t_dlst_node	*dlst_prev(const t_dlst_node *node);
void		*dlst_data(const t_dlst_node *node);

#endif
