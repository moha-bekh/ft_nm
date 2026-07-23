#include "dlst_internal.h"

int dlst_del(t_dlst **dlst, t_dlst_node *node, void (*del)(void *)) {
  void *data;

  if (!dlst || !*dlst || !node)
    return (0);
  if (!(*dlst)->size || node->mgr != *dlst)
    return (0);
  if ((*dlst)->head == node) {
    data = dlst_pop_front(dlst);
    if (del && data)
      del(data);
    return (1);
  }
  if ((*dlst)->tail == node) {
    data = dlst_pop_back(dlst);
    if (del && data)
      del(data);
    return (1);
  }
  node->prev->next = node->next;
  node->next->prev = node->prev;
  if (del && node->data)
    del(node->data);
  free(node);
  (*dlst)->size--;
  return (1);
}
