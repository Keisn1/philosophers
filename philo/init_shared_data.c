/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shared_data.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 10:48/53 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/15 10:48:53 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

t_shared_data	*set_shared_data(t_shared_data *shared, int num_philos)
{
	int	i;

	i = 0;
	while (i < num_philos)
	{
		shared->fork_mutexes[i] = malloc(sizeof(pthread_mutex_t));
		if (!shared->fork_mutexes[i])
			return (teardown_1(shared, i));
		if (pthread_mutex_init(shared->fork_mutexes[i], NULL))
			return (teardown_2(shared, i));
		i++;
	}
	return (shared);
}

void	*set_locks(t_shared_data *shared, int num_philos)
{
	pthread_mutex_t	*check_lock;
	pthread_mutex_t	*stdout_lock;
	pthread_mutex_t	**fork_mutexes;

	check_lock = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(check_lock, NULL);
	shared->check_lock = check_lock;
	stdout_lock = malloc(sizeof(pthread_mutex_t));
	if (!stdout_lock)
		return (teardown_5(shared));
	if (pthread_mutex_init(stdout_lock, NULL))
		return (teardown_4(stdout_lock, shared));
	shared->stdout_lock = stdout_lock;
	fork_mutexes = malloc(sizeof(pthread_mutex_t *) * num_philos);
	if (!fork_mutexes)
		return (teardown_3(stdout_lock, shared));
	shared->fork_mutexes = fork_mutexes;
	return (shared);
}

t_shared_data	*init_shared_data(int num_philos)
{
	t_shared_data	*shared;
	bool			*forks;
	int				i;

	shared = malloc(sizeof(t_shared_data));
	if (!shared)
		return (teardown_6());
	if (!set_locks(shared, num_philos))
		return (NULL);
	forks = malloc(sizeof(bool) * num_philos);
	if (!forks)
	{
		teardown_3(shared->stdout_lock, shared);
		free(shared->fork_mutexes);
		return (NULL);
	}
	i = 0;
	while (i < num_philos)
		forks[i++] = false;
	shared->forks = forks;
	shared->philo_died = false;
	return (set_shared_data(shared, num_philos));
}
