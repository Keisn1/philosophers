/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 09:59/47 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/15 09:59:47 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

unsigned long long	get_timestamp(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (-1);
	return ((unsigned long long)tv.tv_sec * 1000
		+ (unsigned long long)tv.tv_usec / 1000);
}

void	wait_for_base_time(unsigned long long base_time)
{
	while (get_timestamp() < base_time)
		;
}
