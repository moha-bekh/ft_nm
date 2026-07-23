/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moha <moha@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 20:27:12 by mbekheir          #+#    #+#             */
/*   Updated: 2024/08/08 10:16:31 by moha             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr_in(char *str, char *to_ad, char target)
{
	int		i;
	char	*tmp;
	char	*tmp2;
	char	*tmp3;

	if (!str || !to_ad)
		return (NULL);
	i = -1;
	while (str[++i])
	{
		if (str[i] == target && str[i - 1] != '\\')
		{
			tmp = ft_substr(str, 0, i);
			tmp2 = ft_substr(str, i + 1, ft_strlen(str));
			tmp3 = ft_strjoin(tmp, to_ad);
			free(tmp);
			tmp = ft_strjoin(tmp3, tmp2);
			free(tmp2);
			free(tmp3);
			free(str);
			str = ft_strdup(tmp);
			free(tmp);
		}
	}
	return (str);
}
