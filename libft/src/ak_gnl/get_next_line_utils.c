/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbekheir <mbekheir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 17:04:41 by akdovlet          #+#    #+#             */
/*   Updated: 2025/02/21 18:14:24 by mbekheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ak_strlen(char *str)
{
	int	i;

	i = 0;
	if (str)
		while (str[i])
			i++;
	return (i);
}

int	ak_strchr(char *str, char c)
{
	int	i;

	i = -1;
	if (!str)
		return (-1);
	while (str[++i])
		if (str[i] == c)
			return (i);
	return (-1);
}

char	*ak_strjoin(char *s1, char *s2)
{
	char	*fusion;
	int		i;
	int		j;

	i = -1;
	j = -1;
	if (!s1)
	{
		s1 = malloc(sizeof(char) * 1);
		if (!s1)
			return (NULL);
		s1[0] = 0;
	}
	if (!s2)
		return (free(s1), NULL);
	fusion = malloc(sizeof(char) * (ak_strlen(s1) + ak_strlen(s2) + 1));
	if (!fusion)
		return (free(s1), NULL);
	while (s1[++i])
		fusion[i] = s1[i];
	while (s2[++j])
		fusion[i + j] = s2[j];
	fusion[i + j] = 0;
	free(s1);
	return (fusion);
}
