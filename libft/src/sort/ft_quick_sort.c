/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quick_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbekheir <mbekheir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 16:53:06 by mbekheir          #+#    #+#             */
/*   Updated: 2024/05/03 17:34:24 by mbekheir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	partition(int *tab, int start, int end)
{
	int	pivot;
	int	i;
	int	j;

	pivot = tab[end];
	i = (start - 1);
	j = start;
	while (j <= end)
	{
		if (tab[j] < pivot)
		{
			i++;
			ft_swap(&tab[i], &tab[j]);
		}
		j++;
	}
	ft_swap(&tab[i + 1], &tab[end]);
	return (i + 1);
}

void	quick_sort(int *tab, int start, int end)
{
	int	pivot;

	if (start < end)
	{
		pivot = partition(tab, start, end);
		quick_sort(tab, start, pivot - 1);
		quick_sort(tab, pivot + 1, end);
	}
}
