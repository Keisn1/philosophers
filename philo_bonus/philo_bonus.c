/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 14:05/04 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/16 14:05:04 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

bool	check_for_stop(t_meal_check *meal_check)
{
	sem_wait(meal_check->shared.meal_stop_sem);
	if (meal_check->stop_meal_check)
	{
		sem_post(meal_check->shared.meal_stop_sem);
		return (true);
	}
	sem_post(meal_check->shared.meal_stop_sem);
	return (false);
}

void	*meal_check_routine(void *params)
{
	t_meal_check	*meal_check;
	int				count;

	meal_check = (t_meal_check *)params;
	count = 0;
	while (count < meal_check->params.num_philos)
	{
		if (check_for_stop(meal_check))
			return (NULL);
		sem_wait(meal_check->shared.meal_sem);
		count++;
	}
	if (check_for_stop(meal_check))
		return (NULL);
	sem_wait(meal_check->shared.stdout_lock);
	printf("%llu: We have eaten enough\n", get_timestamp()
		- meal_check->params.base_time);
	sem_post(meal_check->shared.dead_lock);
	return (NULL);
}
