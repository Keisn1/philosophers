/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 10:02/17 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/15 10:02:17 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	claim_forks_even(t_philo *philo, bool *has_r_fork, bool *has_l_fork)
{
	if (!*has_r_fork)
	{
		*has_r_fork = try_to_get_r_fork(philo);
		if (*has_r_fork)
			print_fork_msg(philo);
	}
	if (!*has_l_fork && *has_r_fork)
	{
		*has_l_fork = try_to_get_l_fork(philo);
		if (*has_l_fork)
		{
			print_fork_msg(philo);
			set_last_meal(philo, get_timestamp());
		}
	}
}

void	claim_forks_uneven(t_philo *philo, bool *has_r_fork, bool *has_l_fork)
{
	if (!*has_l_fork)
	{
		*has_l_fork = try_to_get_l_fork(philo);
		if (*has_l_fork)
			print_fork_msg(philo);
	}
	if (!*has_r_fork && *has_l_fork)
	{
		*has_r_fork = try_to_get_r_fork(philo);
		if (*has_r_fork)
		{
			print_fork_msg(philo);
			set_last_meal(philo, get_timestamp());
		}
	}
}

bool	get_forks(t_philo *philo)
{
	bool	has_r_fork;
	bool	has_l_fork;

	has_r_fork = false;
	has_l_fork = false;
	while (!has_r_fork || !has_l_fork)
	{
		if (philo->philo_num % 2 == 0)
			claim_forks_even(philo, &has_r_fork, &has_l_fork);
		else
			claim_forks_uneven(philo, &has_r_fork, &has_l_fork);
		if (check_philo_died(philo))
			return (true);
		usleep(100);
	}
	return (false);
}

bool	eat(t_philo *philo)
{
	bool	philo_dead;

	philo_dead = false;
	if (get_forks(philo))
		return (true);
	philo_dead = sleep_loop(philo, get_timestamp(), philo->params.time_to_eat);
	if (philo_dead)
	{
		give_up_forks(philo);
		return (philo_dead);
	}
	philo->meals_eaten++;
	pthread_mutex_lock(philo->shared->check_lock);
	if (philo->meals_eaten == philo->params.must_eat)
		philo->ate_enough = 1;
	pthread_mutex_unlock(philo->shared->check_lock);
	give_up_forks(philo);
	return (false);
}

void	*philo_routine(void *params)
{
	t_philo	*philo;

	philo = (t_philo *)params;
	if (philo->philo_num % 2 == 0)
		usleep(1000);
	while (1)
	{
		if (eat(philo))
			break ;
		if (sleeping(philo))
			break ;
		print_thinking_msg(philo);
		if ((philo->params.time_to_die - (get_timestamp()
					- philo->time_last_meal)) > 10)
			usleep(1000);
	}
	return (NULL);
}
