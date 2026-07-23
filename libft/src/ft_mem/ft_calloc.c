/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbekheir <mbekheir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 13:22:47 by ajordan-          #+#    #+#             */
/*   Updated: 2024/01/26 17:37:28 by mbekheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
*	LIBRARY
*	#include <stdlib.h>
*	DESCRIPTION
*	The calloc() function contiguously allocates enough space for count objects 
*	that are size bytes of memory each and returns a pointer to the allocated 
*	memory.  The allocated memory is filled with bytes of value zero.
*	PARAMETERS
*	#1. The count of objects to allocate.
*	#2. The size of bytes in each object.
*	RETURN VALUES
*	If successful, calloc() function returns a pointer to allocated memory. 
*	If there is an error, they return a NULL pointer.
*/

#include "libft.h"

void	*ft_calloc(size_t data_count, size_t size)
{
	unsigned char	*str;
	size_t			i;

	i = 0;
	if (data_count == 0 || size == 0)
		return (malloc(0));
	if ((data_count * size) < data_count || (data_count * size) < size)
		return (NULL);
	str = malloc(data_count * size);
	if (!str)
		return (NULL);
	while (i < data_count * size)
		str[i++] = 0;
	return (str);
}
