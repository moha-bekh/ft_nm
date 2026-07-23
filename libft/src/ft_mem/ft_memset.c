/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moha <moha@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/21 12:21:58 by ajordan-          #+#    #+#             */
/*   Updated: 2024/02/22 00:12:27 by moha             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 *	LIBRARY
 *	#include <string.h>
 *	DESCRIPTION
 *	The memset() function writes len bytes of value c (converted to an
 *	unsigned char) to the string b.
 *	PARAMETERS
 *	#1. The destiny pointer in which to write.
 *	#2. The character to write.
 *	#3. The number of bytes to write.
 *	RETURN VALUES
 *	The memset() function returns its first argument.
 */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)s)[i] = c;
		i++;
	}
	return (s);
}
