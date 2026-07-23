/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbekheir <mbekheir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 12:01:08 by mbekheir          #+#    #+#             */
/*   Updated: 2023/11/22 09:44:38 by mbekheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	toalloc(int n)
{
	size_t	size;

	size = 0;
	if (n < 0)
		n *= -1;
	while (n != 0)
	{
		n /= 10;
		size++;
	}
	return (size);
}

static char	*new(size_t size)
{
	char	*str;

	str = malloc(sizeof(char) * (size + 1));
	if (!str)
		return (NULL);
	ft_bzero(str, size + 1);
	return (str);
}

char	*ft_itoa(int n)
{
	size_t		i;
	size_t		count;
	long int	nb;
	char		*dst;

	nb = n;
	count = toalloc(n);
	i = 0;
	if (nb < 0 || count == 0)
		count++;
	dst = new (count);
	if (!dst)
		return (NULL);
	if (nb < 0)
	{
		nb *= -1;
		dst[0] = '-';
		i++;
	}
	while (count-- > i)
	{
		dst[count] = (nb % 10) + '0';
		nb /= 10;
	}
	return (dst);
}
