/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 14:28/44 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/16 14:28:44 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo_bonus.h"
#include <semaphore.h>

void	eat(t_philo_data *philo_d)
{
	sem_wait(philo_d->shared.fork_lock);
	sem_wait(philo_d->shared.fork_pile);
	print_fork_msg(philo_d);
	sem_wait(philo_d->shared.fork_pile);
	print_fork_msg(philo_d);
	sem_post(philo_d->shared.fork_lock);
	print_eat_msg(philo_d);
	usleep(1000 * philo_d->params.time_to_eat);
	philo_d->meals_eaten++;
	if (philo_d->meals_eaten == philo_d->params.must_eat)
		sem_post(philo_d->shared.meal_sem);
	sem_post(philo_d->shared.fork_pile);
	sem_post(philo_d->shared.fork_pile);
}

void	sleeping(t_philo_data *philo_d)
{
	print_sleep_msg(philo_d);
	usleep(1000 * philo_d->params.time_to_sleep);
}

void	thinking(t_philo_data *philo_d)
{
	unsigned long long	time_to_think;

	sem_wait(philo_d->shared.check_lock);
	time_to_think = (philo_d->params.time_to_die - (get_timestamp()
				- philo_d->last_meal)) / 2;
	sem_post(philo_d->shared.check_lock);
	usleep(1000 * time_to_think);
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

void	philo_routine(t_philo_data *philo_d)
{
	pthread_t	observer_thread;

	pthread_create(&observer_thread, NULL, observer_routine_time, philo_d);
	pthread_detach(observer_thread);
	wait_for_base_time(philo_d->params.base_time);
	if (philo_d->philo_num % 2 == 0)
		usleep((philo_d->params.time_to_die / 4) * 1000);
	while (1)
	{
		usleep(100 * (philo_d->philo_num / 2));
		eat(philo_d);
		sleeping(philo_d);
		thinking(philo_d);
	}
}
