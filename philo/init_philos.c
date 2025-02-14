/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 09:22/55 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/12 09:22:55 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_shared_data	*set_shared_data(t_shared_data *shared, pthread_mutex_t **fork_mutexes,
		int num_philos, pthread_mutex_t *stdout_lock)
{
	int	i;

	i = 0;
	while (i < num_philos)
	{
		fork_mutexes[i] = malloc(sizeof(pthread_mutex_t));
		if (!fork_mutexes[i])
			return (teardown_1(fork_mutexes, i, stdout_lock, shared));
		if (pthread_mutex_init(fork_mutexes[i], NULL))
			return (teardown_2(fork_mutexes, i, stdout_lock, shared));
		i++;
	}
	return (shared);
}

t_shared_data	*init_shared_data(int num_philos)
{
	t_shared_data	*shared;
	pthread_mutex_t	*stdout_lock;
	pthread_mutex_t	**fork_mutexes;

	shared = malloc(sizeof(t_shared_data));
	if (!shared)
		return (teardown_6());

	pthread_mutex_t *check_lock = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(check_lock, NULL);
	shared->check_lock = check_lock;

	stdout_lock = malloc(sizeof(pthread_mutex_t));
	if (!stdout_lock)
		return (teardown_5(shared));
	if (pthread_mutex_init(stdout_lock, NULL))
		return (teardown_4(stdout_lock, shared));
	fork_mutexes = malloc(sizeof(pthread_mutex_t *) * num_philos);
	if (!fork_mutexes)
		return (teardown_3(stdout_lock, shared));
	shared->fork_mutexes = fork_mutexes;

	shared->philo_died = false;
	shared->stdout_lock = stdout_lock;
	return (set_shared_data(shared, fork_mutexes, num_philos, stdout_lock));
}

t_philo	*set_philo_fork(t_philo *philos, int num_philos, t_shared_data *shared)
{
	int				i;
	pthread_mutex_t	**fork_mutexes;


	i = 0;
	while (i < num_philos)
		philos[i++].shared = shared;

	i = 0;
	fork_mutexes = shared->fork_mutexes;
	while (i < num_philos - 1)
	{
		philos[i].r_fork_mutex = fork_mutexes[i];
		philos[i].l_fork_mutex = fork_mutexes[i + 1];
		i++;
	}
	philos[num_philos - 1].r_fork_mutex = fork_mutexes[num_philos - 1];
	philos[num_philos - 1].l_fork_mutex = fork_mutexes[0];
	return (philos);
}

t_philo	*init_philos(int num_philos)
{
	t_philo			*philos;
	t_shared_data	*shared;

	philos = malloc(sizeof(t_philo) * num_philos);
	if (!philos)
		return (NULL);
	shared = init_shared_data(num_philos);
	if (!shared)
	{
		free(philos);
		return (NULL);
	}
	return (set_philo_fork(philos, num_philos, shared));
}

void set_philo_params(t_philo *philos, int num_philos, t_params params) {
	int i = 0;
	while (i < num_philos) {
		philos[i].philo_num = i+1;
		philos[i].time_last_meal = params.base_time;
		philos[i++].params = params;
	}
}
