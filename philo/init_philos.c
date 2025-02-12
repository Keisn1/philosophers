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

t_shared_data	*set_shared_data(t_shared_data *shared, pthread_mutex_t **forks,
		int num_philos, pthread_mutex_t *stdout_lock)
{
	int	i;

	i = 0;
	while (i < num_philos)
	{
		forks[i] = malloc(sizeof(pthread_mutex_t));
		if (!forks[i])
			return (teardown_1(forks, i, stdout_lock, shared));
		if (pthread_mutex_init(forks[i], NULL))
			return (teardown_2(forks, i, stdout_lock, shared));
		i++;
	}
	shared->philo_died = false;
	shared->stdout_lock = stdout_lock;
	return (shared);
}

t_shared_data	*init_shared_data(int num_philos)
{
	t_shared_data	*shared;
	pthread_mutex_t	*stdout_lock;
	pthread_mutex_t	**forks;

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
	forks = malloc(sizeof(pthread_mutex_t *) * num_philos);
	if (!forks)
		return (teardown_3(stdout_lock, shared));
	shared->forks = forks;
	return (set_shared_data(shared, forks, num_philos, stdout_lock));
}

t_philo	*set_philo_forks(t_philo *philos, int num_philos, t_shared_data *shared)
{
	int				i;
	pthread_mutex_t	**forks;

	i = 0;
	while (i < num_philos)
		philos[i++].shared = shared;

	i = 0;
	forks = shared->forks;
	while (i < num_philos - 1)
	{
		philos[i].r_fork = forks[i];
		philos[i].l_fork = forks[i + 1];
		i++;
	}
	philos[num_philos - 1].r_fork = forks[num_philos - 1];
	philos[num_philos - 1].l_fork = forks[0];
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
	return (set_philo_forks(philos, num_philos, shared));
}

void set_philo_params(t_philo *philos, int num_philos, t_params params) {
	int i = 0;
	while (i < num_philos) {
		philos[i].philo_num = i+1;
		philos[i].time_last_meal = params.base_time;
		philos[i++].params = params;
	}
}
