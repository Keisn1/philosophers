/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 21:52/40 by kfreyer           #+#    #+#             */
/*   Updated: 2024/07/31 21:52:40 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_abs(int x)
{
	if (x < 0)
		return (-x);
	return (x);
}

void	ft_putnbr_ull_fd(unsigned long long nb, int fd)
{
	int		arr[20];
	int		idx;

	idx = 0;
	arr[idx++] = ft_abs(nb % 10);
	nb = ft_abs(nb / 10);
	while (nb > 0)
	{
		arr[idx++] = nb % 10;
		nb /= 10;
	}
	while (--idx >= 0)
		ft_putchar_fd(arr[idx] + '0', fd);
}
