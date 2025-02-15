/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 10:19/35 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/15 10:19:35 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	check_philo_died(t_philo *philo)
{
	pthread_mutex_lock(philo->shared->check_lock);
	if (philo->shared->philo_died)
	{
		give_up_forks(philo);
		pthread_mutex_unlock(philo->shared->check_lock);
		return (true);
	}
	pthread_mutex_unlock(philo->shared->check_lock);
	return (false);
}

void	set_last_meal(t_philo *philo, unsigned long long start)
{
	pthread_mutex_lock(philo->shared->check_lock);
	if (!philo->shared->philo_died)
	{
		philo->time_last_meal = start;
		pthread_mutex_lock(philo->shared->stdout_lock);
		printf("%lld %d is eating\n", philo->time_last_meal
			- philo->params.base_time, philo->philo_num);
		pthread_mutex_unlock(philo->shared->stdout_lock);
	}
	pthread_mutex_unlock(philo->shared->check_lock);
}

bool	sleep_loop(t_philo *philo, unsigned long long start,
		unsigned long long time)
{
	bool	philo_dead;

	philo_dead = false;
	while ((get_timestamp() - start) < time)
	{
		pthread_mutex_lock(philo->shared->check_lock);
		philo_dead = philo->shared->philo_died;
		pthread_mutex_unlock(philo->shared->check_lock);
		if (philo_dead)
			return (true);
		usleep(100);
	}
	return (false);
}

bool	sleeping(t_philo *philo)
{
	unsigned long long	start;
	unsigned long long	time_to_sleep;

	print_sleep_msg(philo);
	start = get_timestamp();
	time_to_sleep = philo->params.time_to_sleep;
	return (sleep_loop(philo, start, time_to_sleep));
}
