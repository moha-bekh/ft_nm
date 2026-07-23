/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbekheir <mbekheir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 18:01:50 by mbekheir          #+#    #+#             */
/*   Updated: 2025/02/21 18:09:03 by mbekheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_new_line(t_list *line_list)
{
	t_list	*current;
	int		i;

	if (!line_list)
		return (0);
	current = get_last_node(line_list);
	i = 0;
	while (current->content[i])
	{
		if (current->content[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

t_list	*get_last_node(t_list *line_list)
{
	t_list	*current;

	current = line_list;
	while (current && current->next)
		current = current->next;
	return (current);
}

void	generate_line(t_list *line_list, char **line)
{
	int	i;
	int	len;

	len = 0;
	while (line_list)
	{
		i = 0;
		while (line_list->content[i])
		{
			if (line_list->content[i] == '\n')
			{
				len++;
				break ;
			}
			len++;
			i++;
		}
		line_list = line_list->next;
	}
	*line = malloc(sizeof(char) * (len + 1));
}

void	free_list(t_list *line_list)
{
	t_list	*current;
	t_list	*next;

	current = line_list;
	while (current)
	{
		free(current->content);
		next = current->next;
		free(current);
		current = next;
	}
}
