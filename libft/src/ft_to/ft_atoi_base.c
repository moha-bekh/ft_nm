/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbekheir <mbekheir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 08:05:29 by moha              #+#    #+#             */
/*   Updated: 2024/10/18 15:38:31 by mbekheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_valid(int c, int base)
{
	char	*lbase;
	char	*ubase;
	int		i;

	lbase = "0123456789abcdef";
	ubase = "0123456789ABCDEF";
	i = -1;
	while (++i < base)
	{
		if (c == lbase[i] || c == ubase[i])
			return (1);
	}
	return (0);
}

void	if_is_sign(int c, int *sign, int *i)
{
	if (c == '+')
		(*i)++;
	else if (c == '-')
	{
		*sign = -1;
		(*i)++;
	}
	return ;
}

int	ft_atoi_base(const char *str, int base)
{
	int	res;
	int	sign;
	int	i;

	res = 0;
	sign = 1;
	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if_is_sign(str[i], &sign, &i);
	while (str[i] && is_valid(str[i], base))
	{
		res *= base;
		if (ft_isdigit(str[i]))
			res += str[i] - '0';
		else if (ft_ishex_alpha(str[i]))
			res += str[i] - 'a' + 10;
		else if (ft_ishex_alpha_(str[i]))
			res += str[i] - 'A' + 10;
		i++;
	}
	return (res * sign);
}
