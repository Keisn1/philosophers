/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_messages.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 09:58/05 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/15 09:58:05 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_fork_msg(t_philo *philo)
{
	pthread_mutex_lock(philo->shared->check_lock);
	if (!philo->shared->philo_died)
	{
		pthread_mutex_lock(philo->shared->stdout_lock);
		printf("%lld %d has taken a fork\n", get_timestamp()
			- philo->params.base_time, philo->philo_num);
		pthread_mutex_unlock(philo->shared->stdout_lock);
	}
	pthread_mutex_unlock(philo->shared->check_lock);
}

void	print_eat_msg(t_philo *philo)
{
	pthread_mutex_lock(philo->shared->check_lock);
	if (!philo->shared->philo_died)
	{
		pthread_mutex_lock(philo->shared->stdout_lock);
		printf("%lld %d is eating\n", get_timestamp() - philo->params.base_time,
			philo->philo_num);
		pthread_mutex_unlock(philo->shared->stdout_lock);
	}
	pthread_mutex_unlock(philo->shared->check_lock);
}

void	print_sleep_msg(t_philo *philo)
{
	pthread_mutex_lock(philo->shared->check_lock);
	if (!philo->shared->philo_died)
	{
		pthread_mutex_lock(philo->shared->stdout_lock);
		printf("%lld %d is sleeping\n", get_timestamp()
			- philo->params.base_time, philo->philo_num);
		pthread_mutex_unlock(philo->shared->stdout_lock);
	}
	pthread_mutex_unlock(philo->shared->check_lock);
}

void	print_thinking_msg(t_philo *philo)
{
	pthread_mutex_lock(philo->shared->check_lock);
	if (!philo->shared->philo_died)
	{
		pthread_mutex_lock(philo->shared->stdout_lock);
		printf("%lld %d is thinking\n", get_timestamp()
			- philo->params.base_time, philo->philo_num);
		pthread_mutex_unlock(philo->shared->stdout_lock);
	}
	pthread_mutex_unlock(philo->shared->check_lock);
}

void	print_philo_died(t_observer *observer, int idx, unsigned long long time)
{
	pthread_mutex_lock(observer->shared->stdout_lock);
	printf("%lld %d died\n", time - observer->params.base_time, idx + 1);
	pthread_mutex_unlock(observer->shared->stdout_lock);
}
