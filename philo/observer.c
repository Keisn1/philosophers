/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   observer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 09:38/58 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/15 09:38:58 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	increment_ate_enough(t_observer *observer, int i)
{
	if (observer->philos[i].ate_enough)
		return (1);
	return (0);
}

bool	check_due_time(t_observer *observer, int i)
{
	unsigned long long	time;
	unsigned long long	time_since_last_meal;

	time = get_timestamp();
	time_since_last_meal = time - observer->philos[i].time_last_meal;
	if (time_since_last_meal >= observer->params.time_to_die)
	{
		observer->shared->philo_died = true;
		print_philo_died(observer, i, time);
		return (true);
	}
	return (false);
}

bool	iterate_over_philos(t_observer *observer)
{
	int		i;
	int		num_philos_who_ate_enough;
	bool	philo_dead;

	i = 0;
	num_philos_who_ate_enough = 0;
	philo_dead = false;
	while (i < observer->num_philos)
	{
		num_philos_who_ate_enough += increment_ate_enough(observer, i);
		if (check_due_time(observer, i))
			return (true);
		i++;
	}
	if (num_philos_who_ate_enough == observer->num_philos)
	{
		observer->shared->philo_died = true;
		philo_dead = true;
	}
	return (philo_dead);
}

void	*observer_routine(void *params)
{
	t_observer	*observer;
	bool		philo_dead;

	observer = (t_observer *)params;
	philo_dead = false;
	while (!philo_dead)
	{
		pthread_mutex_lock(observer->shared->check_lock);
		philo_dead = iterate_over_philos(observer);
		pthread_mutex_unlock(observer->shared->check_lock);
		if (philo_dead)
			break ;
		usleep(100);
	}
	return (NULL);
}
