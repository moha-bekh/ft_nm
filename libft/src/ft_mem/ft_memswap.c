/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memswap.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbekheir <mbekheir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 14:27:08 by mbekheir          #+#    #+#             */
/*   Updated: 2024/05/01 20:01:43 by mbekheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memswap(void *a, void *b, size_t size)
{
	void	*tmp;

	tmp = ft_calloc(1, size);
	if (tmp == NULL)
		return (EXIT_FAILURE);
	ft_memcpy(tmp, a, size);
	ft_memcpy(a, b, size);
	ft_memcpy(b, tmp, size);
	free(tmp);
	return (EXIT_SUCCESS);
}
