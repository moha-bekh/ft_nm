/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbekheir <mbekheir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 05:38:12 by mbekheir          #+#    #+#             */
/*   Updated: 2024/05/01 20:15:17 by mbekheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

bool	is_overflow(long nb, long sign)
{
	long int	x;

	x = nb * sign;
	if (x > INT_MAX || x < INT_MIN)
		return (true);
	return (false);
}

void	set_sign(const char *nptr, int *i, long *sign)
{
	if (nptr[*i] == '+' && nptr[*i + 1] != '-')
		(*i)++;
	else if (nptr[*i] && nptr[*i] == '-')
	{
		if (nptr[*i] == '-')
			*sign = -1;
		(*i)++;
	}
	return ;
}

int	ft_atol(const char *nptr)
{
	int		i;
	long	sign;
	long	nb;

	i = 0;
	sign = 1;
	nb = 0;
	if (ft_strlen(nptr) >= 64)
		return (errno = 1, 0);
	while (nptr[i] && (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13)))
		i++;
	set_sign(&nptr[i], &i, &sign);
	while (nptr[i] && ft_isdigit(nptr[i]))
	{
		nb = nb * 10 + (nptr[i] - '0');
		i++;
	}
	if (is_overflow(nb, sign))
		return (errno = 1, 0);
	if (nptr[i] && !ft_isspace(nptr[i]))
		return (errno = 1, 0);
	return (nb * sign);
}
