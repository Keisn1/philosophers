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
		philos[i].r_fork = &shared->forks[i];
		philos[i].l_fork = &shared->forks[i + 1];
		philos[i].r_fork_mutex = fork_mutexes[i];
		philos[i].l_fork_mutex = fork_mutexes[i + 1];
		i++;
	}
	philos[num_philos - 1].r_fork = &shared->forks[num_philos - 1];
	philos[num_philos - 1].l_fork = &shared->forks[0];
	philos[num_philos - 1].r_fork_mutex = fork_mutexes[num_philos - 1];
	philos[num_philos - 1].l_fork_mutex = fork_mutexes[0];
	return (philos);
}

void	set_philo_params(t_philo *philos, t_params params)
{
	int	i;

	i = 0;
	while (i < params.num_philos)
	{
		philos[i].philo_num = i + 1;
		philos[i].time_last_meal = params.base_time;
		philos[i].meals_eaten = 0;
		philos[i].ate_enough = false;
		philos[i++].params = params;
	}
}

t_philo	*init_philos(int num_philos, t_params params)
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
	philos = set_philo_fork(philos, num_philos, shared);
	set_philo_params(philos, params);
	return (philos);
}
