#include "../src/dlst_internal.h"
#include <stdio.h>

typedef struct s_test_ctx
{
	int	total;
	int	failed;
	int	current_failed;
}	t_test_ctx;

static t_test_ctx	g_test;
static int			g_deleted;
static int			g_foreach_values[32];
static int			g_foreach_count;

static void	test_begin(const char *name)
{
	g_test.current_failed = 0;
	printf("\n[TEST] %s\n", name);
}

static void	test_end(void)
{
	if (!g_test.current_failed)
		printf("  OK\n");
}

static void	expect_true(int condition, const char *message)
{
	g_test.total++;
	if (condition)
		return ;
	g_test.failed++;
	g_test.current_failed++;
	printf("  FAIL: %s\n", message);
}

static int	*new_int(int value)
{
	int	*data;

	data = malloc(sizeof(int));
	if (!data)
		return (NULL);
	*data = value;
	return (data);
}

static void	delete_int(void *data)
{
	g_deleted++;
	free(data);
}

static void	remember_int(void *data)
{
	if (g_foreach_count < 32)
		g_foreach_values[g_foreach_count++] = *(int *)data;
}

static int	node_value(t_dlst_node *node)
{
	return (*(int *)dlst_data(node));
}

static int	cmp_int(void *a, void *b)
{
	return (*(int *)a - *(int *)b);
}

static void	expect_list(t_dlst *list, const int *values, size_t size,
		const char *context)
{
	t_dlst_node	*node;
	t_dlst_node	*prev;
	size_t		count;
	char		message[160];

	snprintf(message, sizeof(message), "%s: list is not NULL", context);
	expect_true(list != NULL, message);
	if (!list)
		return ;
	snprintf(message, sizeof(message), "%s: size == %zu", context, size);
	expect_true(dlst_size(list) == size, message);
	if (size == 0)
	{
		snprintf(message, sizeof(message), "%s: head is NULL", context);
		expect_true(dlst_head(list) == NULL, message);
		snprintf(message, sizeof(message), "%s: tail is NULL", context);
		expect_true(dlst_tail(list) == NULL, message);
		return ;
	}
	snprintf(message, sizeof(message), "%s: head exists", context);
	expect_true(dlst_head(list) != NULL, message);
	snprintf(message, sizeof(message), "%s: tail exists", context);
	expect_true(dlst_tail(list) != NULL, message);
	if (!dlst_head(list) || !dlst_tail(list))
		return ;
	snprintf(message, sizeof(message), "%s: head->prev is NULL", context);
	expect_true(dlst_prev(dlst_head(list)) == NULL, message);
	snprintf(message, sizeof(message), "%s: tail->next is NULL", context);
	expect_true(dlst_next(dlst_tail(list)) == NULL, message);
	node = dlst_head(list);
	prev = NULL;
	count = 0;
	while (node && count <= size + 2)
	{
		snprintf(message, sizeof(message), "%s: node %zu prev link", context,
			count);
		expect_true(dlst_prev(node) == prev, message);
		snprintf(message, sizeof(message), "%s: node %zu mgr link", context,
			count);
		expect_true(node->mgr == list, message);
		if (count < size)
		{
			snprintf(message, sizeof(message), "%s: node %zu value == %d",
				context, count, values[count]);
			expect_true(node_value(node) == values[count], message);
		}
		prev = node;
		node = dlst_next(node);
		count++;
	}
	snprintf(message, sizeof(message), "%s: forward count == %zu", context,
		size);
	expect_true(count == size, message);
	snprintf(message, sizeof(message), "%s: last forward node is tail",
		context);
	expect_true(prev == dlst_tail(list), message);
}

static t_dlst	*list_from_values(const int *values, size_t size)
{
	t_dlst	*list;
	size_t	i;

	list = NULL;
	i = 0;
	while (i < size)
	{
		expect_true(dlst_push_back(&list, new_int(values[i])),
			"list_from_values push_back succeeds");
		i++;
	}
	return (list);
}

static void	test_dlst_new_case(void)
{
	t_dlst	*list;

	test_begin("dlst_new");
	list = dlst_new();
	expect_true(list != NULL, "returns allocated list");
	expect_true(dlst_head(list) == NULL, "head initialized to NULL");
	expect_true(dlst_tail(list) == NULL, "tail initialized to NULL");
	expect_true(dlst_size(list) == 0, "size initialized to 0");
	free(list);
	test_end();
}

static void	test_dlst_node_new_case(void)
{
	t_dlst_node	*node;

	test_begin("dlst_node_new internal");
	node = dlst_node_new();
	expect_true(node != NULL, "returns allocated node");
	expect_true(dlst_next(node) == NULL, "next initialized to NULL");
	expect_true(dlst_prev(node) == NULL, "prev initialized to NULL");
	expect_true(node->mgr == NULL, "mgr initialized to NULL");
	expect_true(dlst_data(node) == NULL, "data initialized to NULL");
	free(node);
	test_end();
}

static void	test_push_back_case(void)
{
	t_dlst		*list;
	const int	expected1[] = {10};
	const int	expected2[] = {10, 20};

	test_begin("dlst_push_back");
	list = NULL;
	expect_true(!dlst_push_back(NULL, new_int(1)), "NULL list pointer fails");
	expect_true(dlst_push_back(&list, new_int(10)), "push_back on NULL list succeeds");
	expect_list(list, expected1, 1, "push_back on NULL list");
	expect_true(dlst_push_back(&list, new_int(20)), "push_back on existing list succeeds");
	expect_list(list, expected2, 2, "push_back on existing list");
	dlst_clear(&list, delete_int);
	expect_true(list == NULL, "clear sets list to NULL");
	test_end();
}

static void	test_push_front_case(void)
{
	t_dlst		*list;
	const int	expected1[] = {10};
	const int	expected2[] = {20, 10};

	test_begin("dlst_push_front");
	list = NULL;
	expect_true(!dlst_push_front(NULL, new_int(1)), "NULL list pointer fails");
	expect_true(dlst_push_front(&list, new_int(10)), "push_front on NULL list succeeds");
	expect_list(list, expected1, 1, "push_front on NULL list");
	expect_true(dlst_push_front(&list, new_int(20)), "push_front on existing list succeeds");
	expect_list(list, expected2, 2, "push_front on existing list");
	dlst_clear(&list, delete_int);
	expect_true(list == NULL, "clear sets list to NULL");
	test_end();
}

static void	test_insert_next_case(void)
{
	t_dlst		*list;
	t_dlst		*other;
	const int	base[] = {1, 3};
	const int	middle[] = {1, 2, 3};
	const int	after_tail[] = {1, 2, 3, 4};

	test_begin("dlst_insert_next");
	list = NULL;
	expect_true(!dlst_insert_next(NULL, NULL, new_int(1)),
		"NULL list pointer fails");
	expect_true(dlst_insert_next(&list, NULL, new_int(42)),
		"insert_next on NULL list succeeds");
	expect_list(list, (const int []){42}, 1, "insert_next on NULL list");
	dlst_clear(&list, delete_int);
	list = list_from_values(base, 2);
	expect_true(dlst_insert_next(&list, dlst_head(list), new_int(2)),
		"insert_next after head succeeds");
	expect_list(list, middle, 3, "insert_next after head");
	expect_true(dlst_insert_next(&list, dlst_tail(list), new_int(4)),
		"insert_next after tail succeeds");
	expect_list(list, after_tail, 4, "insert_next after tail");
	other = list_from_values((const int []){9}, 1);
	expect_true(!dlst_insert_next(&list, dlst_head(other), new_int(99)),
		"insert_next rejects node from another list");
	dlst_clear(&other, delete_int);
	dlst_clear(&list, delete_int);
	test_end();
}

static void	test_insert_prev_case(void)
{
	t_dlst		*list;
	t_dlst		*other;
	const int	base[] = {1, 3};
	const int	middle[] = {1, 2, 3};
	const int	before_head[] = {0, 1, 2, 3};

	test_begin("dlst_insert_prev");
	list = NULL;
	expect_true(!dlst_insert_prev(NULL, NULL, new_int(1)),
		"NULL list pointer fails");
	expect_true(dlst_insert_prev(&list, NULL, new_int(42)),
		"insert_prev on NULL list succeeds");
	expect_list(list, (const int []){42}, 1, "insert_prev on NULL list");
	dlst_clear(&list, delete_int);
	list = list_from_values(base, 2);
	expect_true(dlst_insert_prev(&list, dlst_tail(list), new_int(2)),
		"insert_prev before tail succeeds");
	expect_list(list, middle, 3, "insert_prev before tail");
	expect_true(dlst_insert_prev(&list, dlst_head(list), new_int(0)),
		"insert_prev before head succeeds");
	expect_list(list, before_head, 4, "insert_prev before head");
	other = list_from_values((const int []){9}, 1);
	expect_true(!dlst_insert_prev(&list, dlst_head(other), new_int(99)),
		"insert_prev rejects node from another list");
	dlst_clear(&other, delete_int);
	dlst_clear(&list, delete_int);
	test_end();
}

static void	test_pop_front_case(void)
{
	t_dlst		*list;
	void		*data;
	const int	values[] = {1, 2, 3};
	const int	expected[] = {2, 3};

	test_begin("dlst_pop_front");
	expect_true(dlst_pop_front(NULL) == NULL, "NULL list pointer returns NULL");
	list = NULL;
	expect_true(dlst_pop_front(&list) == NULL, "NULL list returns NULL");
	list = list_from_values(values, 3);
	data = dlst_pop_front(&list);
	expect_true(*(int *)data == 1, "pop_front returns head data");
	delete_int(data);
	expect_list(list, expected, 2, "pop_front removes head");
	delete_int(dlst_pop_front(&list));
	delete_int(dlst_pop_front(&list));
	expect_true(list == NULL, "last pop_front frees list and sets NULL");
	test_end();
}

static void	test_pop_back_case(void)
{
	t_dlst		*list;
	void		*data;
	const int	values[] = {1, 2, 3};
	const int	expected[] = {1, 2};

	test_begin("dlst_pop_back");
	expect_true(dlst_pop_back(NULL) == NULL, "NULL list pointer returns NULL");
	list = NULL;
	expect_true(dlst_pop_back(&list) == NULL, "NULL list returns NULL");
	list = list_from_values(values, 3);
	data = dlst_pop_back(&list);
	expect_true(*(int *)data == 3, "pop_back returns tail data");
	delete_int(data);
	expect_list(list, expected, 2, "pop_back removes tail");
	delete_int(dlst_pop_back(&list));
	delete_int(dlst_pop_back(&list));
	expect_true(list == NULL, "last pop_back frees list and sets NULL");
	test_end();
}

static void	test_del_case(void)
{
	t_dlst		*list;
	t_dlst		*other;
	const int	values[] = {1, 2, 3};
	const int	after_middle[] = {1, 3};
	const int	after_head[] = {3};

	test_begin("dlst_del");
	expect_true(!dlst_del(NULL, NULL, delete_int), "NULL list pointer fails");
	list = list_from_values(values, 3);
	expect_true(!dlst_del(&list, NULL, delete_int), "NULL node fails");
	other = list_from_values((const int []){9}, 1);
	expect_true(!dlst_del(&list, dlst_head(other), delete_int),
		"node from another list fails");
	dlst_clear(&other, delete_int);
	g_deleted = 0;
	expect_true(dlst_del(&list, dlst_next(dlst_head(list)), delete_int),
		"del middle node succeeds");
	expect_true(g_deleted == 1, "delete callback called for middle node");
	expect_list(list, after_middle, 2, "del middle node");
	expect_true(dlst_del(&list, dlst_head(list), delete_int),
		"del head node succeeds");
	expect_list(list, after_head, 1, "del head node");
	expect_true(dlst_del(&list, dlst_tail(list), delete_int),
		"del tail/last node succeeds");
	expect_true(list == NULL, "del last node frees list");
	expect_true(g_deleted == 3, "delete callback called for every deleted node");
	test_end();
}

static void	test_clear_case(void)
{
	t_dlst		*list;
	const int	values[] = {1, 2, 3, 4};

	test_begin("dlst_clear");
	dlst_clear(NULL, delete_int);
	expect_true(1, "clear with NULL list pointer is a no-op");
	g_deleted = 0;
	list = list_from_values(values, 4);
	dlst_clear(&list, delete_int);
	expect_true(list == NULL, "clear sets list to NULL");
	expect_true(g_deleted == 4, "delete callback called for every node");
	test_end();
}

static void	test_foreach_case(void)
{
	t_dlst		*list;
	const int	values[] = {4, 5, 6};

	test_begin("dlst_foreach");
	dlst_foreach(NULL, remember_int);
	expect_true(1, "foreach with NULL list is a no-op");
	list = list_from_values(values, 3);
	dlst_foreach(list, NULL);
	expect_list(list, values, 3, "NULL callback leaves list unchanged");
	g_foreach_count = 0;
	dlst_foreach(list, remember_int);
	expect_true(g_foreach_count == 3, "callback called three times");
	expect_true(g_foreach_values[0] == 4, "foreach value 0");
	expect_true(g_foreach_values[1] == 5, "foreach value 1");
	expect_true(g_foreach_values[2] == 6, "foreach value 2");
	dlst_clear(&list, delete_int);
	test_end();
}

static void	test_null_delete_callback_case(void)
{
	t_dlst	*list;
	int		values[] = {1, 2, 3};

	test_begin("NULL delete callback");
	list = NULL;
	dlst_push_back(&list, &values[0]);
	dlst_push_back(&list, &values[1]);
	dlst_push_back(&list, &values[2]);
	expect_true(dlst_del(&list, dlst_head(list), NULL),
		"del with NULL del succeeds");
	expect_list(list, (const int []){2, 3}, 2, "del with NULL del");
	dlst_clear(&list, NULL);
	expect_true(list == NULL, "clear with NULL del returns NULL");
	expect_true(values[0] == 1 && values[1] == 2 && values[2] == 3,
		"NULL del does not free or modify data");
	test_end();
}

static void	test_empty_dlst_new_operations(void)
{
	t_dlst	*list;
	void	*data;

	test_begin("operations on empty dlst_new list");
	list = dlst_new();
	expect_true(dlst_push_back(&list, new_int(1)),
		"push_back on empty dlst_new list succeeds");
	expect_list(list, (const int []){1}, 1, "push_back on empty dlst_new list");
	dlst_clear(&list, delete_int);
	list = dlst_new();
	expect_true(dlst_push_front(&list, new_int(1)),
		"push_front on empty dlst_new list succeeds");
	expect_list(list, (const int []){1}, 1, "push_front on empty dlst_new list");
	dlst_clear(&list, delete_int);
	list = dlst_new();
	data = dlst_pop_front(&list);
	expect_true(data == NULL, "pop_front on empty dlst_new list returns NULL");
	expect_true(list == NULL, "pop_front on empty dlst_new list frees list");
	list = dlst_new();
	data = dlst_pop_back(&list);
	expect_true(data == NULL, "pop_back on empty dlst_new list returns NULL");
	expect_true(list == NULL, "pop_back on empty dlst_new list frees list");
	list = dlst_new();
	dlst_clear(&list, delete_int);
	expect_true(list == NULL, "clear on empty dlst_new list returns NULL");
	test_end();
}

static void	test_insert_null_node_case(void)
{
	t_dlst	*list;

	test_begin("insert with NULL node");
	list = NULL;
	expect_true(dlst_push_back(&list, new_int(1)), "initial push succeeds");
	expect_true(dlst_insert_next(&list, NULL, new_int(2)),
		"insert_next with NULL node succeeds");
	expect_list(list, (const int []){1, 2}, 2,
		"insert_next with NULL node pushes back");
	dlst_clear(&list, delete_int);
	list = NULL;
	expect_true(dlst_push_back(&list, new_int(1)), "initial push succeeds");
	expect_true(dlst_insert_prev(&list, NULL, new_int(2)),
		"insert_prev with NULL node succeeds");
	expect_list(list, (const int []){2, 1}, 2,
		"insert_prev with NULL node pushes front");
	dlst_clear(&list, delete_int);
	test_end();
}

static void	test_dlst_sort_case(void)
{
	t_dlst	*list;

	test_begin("dlst_sort");
	list = list_from_values((const int []){4, 2, 5, 1, 2}, 5);
	dlst_sort(list, cmp_int);
	expect_list(list, (const int []){1, 2, 2, 4, 5}, 5,
		"sort ascending");
	dlst_sort(NULL, cmp_int);
	dlst_sort(list, NULL);
	expect_list(list, (const int []){1, 2, 2, 4, 5}, 5,
		"sort guards keep list unchanged");
	dlst_clear(&list, delete_int);
	test_end();
}

int	main(void)
{
	setvbuf(stdout, NULL, _IONBF, 0);
	test_dlst_new_case();
	test_dlst_node_new_case();
	test_push_back_case();
	test_push_front_case();
	test_insert_next_case();
	test_insert_prev_case();
	test_pop_front_case();
	test_pop_back_case();
	test_del_case();
	test_clear_case();
	test_foreach_case();
	test_null_delete_callback_case();
	test_empty_dlst_new_operations();
	test_insert_null_node_case();
	test_dlst_sort_case();
	printf("\nAssertions: %d total, %d failed\n", g_test.total, g_test.failed);
	if (g_test.failed)
		return (1);
	return (0);
}
