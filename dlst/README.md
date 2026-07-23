# libdlst

`libdlst` is a small C library for doubly linked lists.
It stores `void *` pointers, so it can be used with any data type.

## Diagram

```text
                  t_dlst
        +-----------------------+
        | head              tail|
        |  |                 |  |
        |  v                 v  |
NULL <- [A] <-> [B] <-> [C] -> NULL
        |       |       |
        v       v       v
      data    data    data
```

Each node stores its previous node, next node, parent list, and user data.

## Build

```sh
make
```

This command builds the static library:

```text
libdlst.a
```

Useful commands:

```sh
make clean    # remove object files
make fclean   # remove object files and libdlst.a
make re       # rebuild from scratch
make test     # build and run tests
```

## Use in a Project

Include the public header:

```c
#include "dlst.h"
```

Compile with the `inc` directory and link with `libdlst.a`:

```sh
cc main.c -I inc libdlst.a -o my_program
```

## Quick Example

```c
#include "dlst.h"
#include <stdio.h>
#include <stdlib.h>

static void	print_int(void *data)
{
	printf("%d\n", *(int *)data);
}

static void	delete_int(void *data)
{
	free(data);
}

static int	*new_int(int value)
{
	int	*ptr;

	ptr = malloc(sizeof(int));
	if (!ptr)
		return (NULL);
	*ptr = value;
	return (ptr);
}

int	main(void)
{
	t_dlst	*list;

	list = NULL;
	dlst_push_back(&list, new_int(10));
	dlst_push_back(&list, new_int(20));
	dlst_push_front(&list, new_int(5));

	dlst_foreach(list, print_int);
	dlst_clear(&list, delete_int);
	return (0);
}
```

Output:

```text
5
10
20
```

## Public API

### Creation and Destruction

```c
t_dlst	*dlst_new(void);
void	dlst_clear(t_dlst **dlst, void (*del)(void *));
```

- `dlst_new` creates an empty list.
- `dlst_clear` deletes every node, calls `del` on each data pointer when
  `del` is not `NULL`, then sets the list pointer to `NULL`.
- You can also start with `t_dlst *list = NULL`; insertion functions will
  create the list automatically.

### Insertion

```c
int	dlst_push_back(t_dlst **dlst, void *data);
int	dlst_push_front(t_dlst **dlst, void *data);
int	dlst_insert_next(t_dlst **dlst, t_dlst_node *node, void *data);
int	dlst_insert_prev(t_dlst **dlst, t_dlst_node *node, void *data);
```

- These functions return `1` on success and `0` on error.
- `dlst_push_back` inserts at the end of the list.
- `dlst_push_front` inserts at the beginning of the list.
- `dlst_insert_next` inserts after `node`.
- `dlst_insert_prev` inserts before `node`.
- If `node == NULL`, `dlst_insert_next` inserts at the end of the list and
  `dlst_insert_prev` inserts at the beginning of the list.

### Removal

```c
void	*dlst_pop_front(t_dlst **dlst);
void	*dlst_pop_back(t_dlst **dlst);
int		dlst_del(t_dlst **dlst, t_dlst_node *node, void (*del)(void *));
```

- `dlst_pop_front` removes the first node and returns its data pointer.
- `dlst_pop_back` removes the last node and returns its data pointer.
- The `pop` functions do not free the returned data: the caller remains
  responsible for it.
- `dlst_del` removes a specific node and calls `del` on its data pointer when
  `del` is not `NULL`.
- When the last node is removed, the list is freed and the list pointer is set
  to `NULL`.

### Traversal and Access

```c
void		dlst_foreach(t_dlst *dlst, void (*f)(void *));
size_t		dlst_size(const t_dlst *dlst);
t_dlst_node	*dlst_head(const t_dlst *dlst);
t_dlst_node	*dlst_tail(const t_dlst *dlst);
t_dlst_node	*dlst_next(const t_dlst_node *node);
t_dlst_node	*dlst_prev(const t_dlst_node *node);
void		*dlst_data(const t_dlst_node *node);
```

The internal structures are opaque to users. To traverse the list manually,
use the getters:

```c
t_dlst_node	*node;

node = dlst_head(list);
while (node)
{
	printf("%p\n", dlst_data(node));
	node = dlst_next(node);
}
```

## Memory Management

The list does not know the real type of the stored data, so it cannot free it
by itself.

- If the data is dynamically allocated, provide a `del` function.
- If the data should not be freed by the list, pass `NULL`.
- After a `pop`, the returned data still belongs to the caller.

## Full Example

A more complete example is available in:

```text
src/dlst_example.c
```
