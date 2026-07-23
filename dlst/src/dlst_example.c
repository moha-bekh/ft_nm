#include "dlst.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_token
{
	char	*value;
	char	type;
	bool	flag;
}	t_token;

static void	token_print(void *data)
{
	t_token	*token;

	token = data;
	if (!token)
		return ;
	printf("type: %c \tflag: %d \tvalue: %s\n", token->type, token->flag,
		token->value);
}

static t_token	*token_new(char *value, char type, bool flag)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	*token = (t_token){0};
	if (value)
		token->value = strdup(value);
	token->type = type;
	token->flag = flag;
	return (token);
}

static void	token_clear(void *data)
{
	t_token	*token;

	token = data;
	if (!token)
		return ;
	free(token->value);
	free(token);
}

static void	token_free_value(void *data)
{
	t_token	*token;

	token = data;
	if (!token)
		return ;
	free(token->value);
	token->value = NULL;
}

int	main(void)
{
	t_dlst	*dlst;

	dlst = NULL;
	dlst_push_front(&dlst, token_new("'world!'", '\'', false));
	dlst_push_back(&dlst, token_new(">", '>', true));
	dlst_push_front(&dlst, token_new("\"hello\"", '"', false));
	dlst_push_back(&dlst, token_new("out", 'S', true));
	dlst_push_front(&dlst, token_new("echo", 'S', false));
	dlst_push_back(&dlst, token_new(";", ';', true));
	dlst_insert_next(&dlst, dlst_head(dlst), token_new("-n", 'S', true));
	dlst_insert_prev(&dlst, dlst_tail(dlst), token_new("&&", '&', false));
	dlst_insert_prev(&dlst, dlst_tail(dlst), token_new("ls", 'S', false));
	dlst_foreach(dlst, token_print);
	dlst_foreach(dlst, token_free_value);
	dlst_foreach(dlst, token_print);
	dlst_clear(&dlst, token_clear);
	return (0);
}
