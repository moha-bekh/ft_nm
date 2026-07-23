/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbekheir <mbekheir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:03:37 by mbekheir          #+#    #+#             */
/*   Updated: 2025/04/15 14:57:33 by mbekheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_buf(t_ft_dprintf *d, char c)
{
	if (d->j == FT_DPRINTF_BUFF_SIZE - 1)
	{
		write(d->fd, d->buf, d->j);
		d->j = 0;
	}
	d->buf[d->j++] = c;
	d->len++;
	return ;
}

void	ft_putstr_buf(t_ft_dprintf *d, char *str)
{
	int	idx;

	idx = -1;
	while (str[++idx])
		ft_putchar_buf(d, str[idx]);
}
