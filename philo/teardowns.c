/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_teardowns.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 09:37/08 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/15 09:37:08 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	teardown_main(t_philo *philos, int num_philos, pthread_t *threads)
{
	int				i;
	pthread_mutex_t	**all_forks;

	i = 0;
	all_forks = philos[0].shared->fork_mutexes;
	while (i < num_philos)
	{
		pthread_mutex_destroy(all_forks[i]);
		free(all_forks[i++]);
	}
	free(all_forks);
	pthread_mutex_destroy(philos[0].shared->stdout_lock);
	free(philos[0].shared->forks);
	free(philos[0].shared->stdout_lock);
	free(philos[0].shared->check_lock);
	free(philos[0].shared);
	free(philos);
	free(threads);
}

void	*teardown_1(pthread_mutex_t **forks, int i,
		pthread_mutex_t *stdout_lock, t_shared_data *shared)
{
	printf("Allocation failed\n");
	while (i-- > 0)
	{
		pthread_mutex_destroy(forks[i]);
		free(forks[i]);
	}
	free(forks);
	pthread_mutex_destroy(stdout_lock);
	free(stdout_lock);
	free(shared);
	return (NULL);
}

void	*teardown_2(pthread_mutex_t **forks, int i,
		pthread_mutex_t *stdout_lock, t_shared_data *shared)
{
	printf("Mutex initialization failed\n");
	free(forks[i]);
	while (i-- > 0)
	{
		pthread_mutex_destroy(forks[i]);
		free(forks[i]);
	}
	free(forks);
	pthread_mutex_destroy(stdout_lock);
	free(stdout_lock);
	free(shared);
	return (NULL);
}
