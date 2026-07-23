/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbekheir <mbekheir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/22 12:48:23 by ajordan-          #+#    #+#             */
/*   Updated: 2024/01/26 17:50:06 by mbekheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *str1, const void *str2, size_t n)
{
	unsigned char	*strr1;
	unsigned char	*strr2;
	size_t			i;

	if (n != 0)
	{
		strr1 = (unsigned char *)str1;
		strr2 = (unsigned char *)str2;
		i = 0;
		while (i < n)
		{
			if (strr1[i] != strr2[i])
				return (strr1[i] - strr2[i]);
			i++;
		}
	}
	return (0);
}
