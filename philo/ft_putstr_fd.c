/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 21:48/44 by kfreyer           #+#    #+#             */
/*   Updated: 2024/07/31 21:48:44 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_putstr_fd(char *s, int fd)
{
	size_t	count;

	count = 0;
	while (*s)
		count += ft_putchar_fd(*s++, fd);
	return (count);
}
