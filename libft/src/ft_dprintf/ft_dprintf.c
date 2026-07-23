/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbekheir <mbekheir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:03:32 by mbekheir          #+#    #+#             */
/*   Updated: 2025/04/15 14:57:18 by mbekheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putunbrbase_buf(t_ft_dprintf *d, unsigned long nb,
		unsigned long base,
		bool upper)
{
	if (!nb)
		ft_putchar_buf(d, '0');
	else
	{
		if (nb >= base)
			ft_putunbrbase_buf(d, (nb / base), base, upper);
		if ((nb % base) < 10)
			ft_putchar_buf(d, (nb % base) + '0');
		else
		{
			if (upper)
				ft_putchar_buf(d, (nb % base) - 10 + 'A');
			else
				ft_putchar_buf(d, (nb % base) - 10 + 'a');
		}
	}
}

void	ft_putnbrbase_buf(t_ft_dprintf *d, long nb, unsigned long base,
		bool upper)
{
	if (nb < 0)
	{
		ft_putchar_buf(d, '-');
		ft_putunbrbase_buf(d, (unsigned long)(-(nb + 1)) + 1, base, upper);
	}
	else
		ft_putunbrbase_buf(d, (unsigned long)nb, base, upper);
}

void	ft_putptr_rec(t_ft_dprintf *d, unsigned long long ptr)
{
	if (ptr >= 16)
		ft_putptr_rec(d, (ptr / 16));
	if ((ptr % 16) < 10)
		ft_putchar_buf(d, (ptr % 16) + '0');
	else
		ft_putchar_buf(d, (ptr % 16) - 10 + 'a');
}

void	ft_putptr_buf(t_ft_dprintf *d, unsigned long long ptr)
{
	if (!ptr)
		ft_putstr_buf(d, "(nil)");
	else
	{
		ft_putstr_buf(d, "0x");
		ft_putptr_rec(d, ptr);
	}
}

void	ft_putformat(t_ft_dprintf *d)
{
	d->i++;
	if (d->format[d->i] == '%')
		ft_putchar_buf(d, '%');
	else if (d->format[d->i] == 'c')
		ft_putchar_buf(d, va_arg(d->args, int));
	else if (d->format[d->i] == 's')
		ft_putstr_buf(d, va_arg(d->args, char *));
	else if (d->format[d->i] == 'd' || d->format[d->i] == 'i')
		ft_putnbrbase_buf(d, va_arg(d->args, int), 10, false);
	else if (d->format[d->i] == 'u')
		ft_putunbrbase_buf(d, va_arg(d->args, unsigned int), 10, false);
	else if (d->format[d->i] == 'x')
		ft_putunbrbase_buf(d, va_arg(d->args, unsigned int), 16, false);
	else if (d->format[d->i] == 'X')
		ft_putunbrbase_buf(d, va_arg(d->args, unsigned int), 16, true);
	else if (d->format[d->i] == 'p')
		ft_putptr_buf(d, va_arg(d->args, unsigned long long));
	else if (d->format[d->i] == 'l')
	{
		d->i++;
		if (d->format[d->i] == 'd' || d->format[d->i] == 'i')
			ft_putnbrbase_buf(d, va_arg(d->args, long), 10, false);
		else if (d->format[d->i] == 'u')
			ft_putunbrbase_buf(d, va_arg(d->args, unsigned long), 10, false);
		else if (d->format[d->i] == 'x')
			ft_putunbrbase_buf(d, va_arg(d->args, unsigned long), 16, false);
		else if (d->format[d->i] == 'X')
			ft_putunbrbase_buf(d, va_arg(d->args, unsigned long), 16, true);
	}
	return ;
}

int	ft_dprintf(int fd, const char *format, ...)
{
	t_ft_dprintf	d;

	d = (t_ft_dprintf){0};
	d.fd = fd;
	d.format = format;
	va_start(d.args, format);
	while (d.format[d.i])
	{
		if (d.format[d.i] == '%')
			ft_putformat(&d);
		else
			ft_putchar_buf(&d, d.format[d.i]);
		d.i++;
	}
	write(d.fd, d.buf, d.j);
	va_end(d.args);
	return (d.len);
}
