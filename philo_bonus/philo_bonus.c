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

void	*meal_check_routine(void *params)
{
	t_meal_check	*meal_check;
	int				count;

	meal_check = (t_meal_check *)params;
	count = 0;
	while (count < meal_check->params.num_philos)
	{
		sem_wait(meal_check->shared.meal_sem);
		count++;
	}
	sem_wait(meal_check->shared.stdout_lock);
	printf("%llu: We have eaten enough\n", get_timestamp()
		- meal_check->params.base_time);
	sem_post(meal_check->shared.dead_lock);
	return (NULL);
}

void	*observer_routine_time(void *params)
{
	t_philo_data		*philo_d;
	unsigned long long	time;

	philo_d = (t_philo_data *)params;
	while (1)
	{
		sem_wait(philo_d->shared.check_lock);
		wait_for_base_time(philo_d->params.base_time);
		time = get_timestamp();
		if (time - philo_d->last_meal >= philo_d->params.time_to_die)
		{
			sem_wait(philo_d->shared.stdout_lock);
			printf("%lld %d died\n", time - philo_d->params.base_time,
				philo_d->philo_num);
			sem_post(philo_d->shared.dead_lock);
			return (NULL);
		}
		sem_post(philo_d->shared.check_lock);
		usleep(100);
	}
	return (NULL);
}
