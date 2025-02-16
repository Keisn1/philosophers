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
		time = get_timestamp();
		if (time < philo_d->last_meal)
		{
			sem_post(philo_d->shared.check_lock);
			usleep(100);
			continue ;
		}
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
void	philo_routine(t_philo_data *philo_d)
{
	pthread_t	sub_thread;

	pthread_create(&sub_thread, NULL, observer_routine_time, philo_d);
	while (get_timestamp() < philo_d->params.base_time)
	{
		usleep(100);
	}
	while (1)
	{
		/* eating */
		sem_wait(philo_d->shared.fork_lock);
		sem_wait(philo_d->shared.fork_pile);
		print_fork_msg(philo_d);
		sem_wait(philo_d->shared.fork_pile);
		print_fork_msg(philo_d);
		sem_post(philo_d->shared.fork_lock);
		print_eat_msg(philo_d);
		while ((get_timestamp()
				- philo_d->last_meal) < philo_d->params.time_to_eat)
		{
			usleep(100);
		}
		philo_d->meals_eaten++;
		if (philo_d->meals_eaten == philo_d->params.must_eat)
		{
			sem_post(philo_d->shared.meal_sem);
		}
		sem_post(philo_d->shared.fork_pile);
		sem_post(philo_d->shared.fork_pile);
		/* sleeping */
		print_sleep_msg(philo_d);
		while ((get_timestamp()
				- philo_d->last_meal) < (philo_d->params.time_to_eat
				+ philo_d->params.time_to_sleep))
		{
			usleep(100);
		}
		/* thinking */
		print_thinking_msg(philo_d);
		if ((philo_d->params.time_to_die - (get_timestamp()
					- philo_d->last_meal)) > 10)
			usleep(1000);
	}
}
