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

void	teardown_main(t_philo *philos, pthread_t *threads)
{
	int				i;
	pthread_mutex_t	**all_forks;

	i = 0;
	all_forks = philos[0].shared->fork_mutexes;
	while (i < philos[0].params.num_philos)
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

void	*teardown_1(t_shared_data *shared, int i)
{
	printf("Allocation failed\n");
	while (i-- > 0)
	{
		pthread_mutex_destroy(shared->fork_mutexes[i]);
		free(shared->fork_mutexes[i]);
	}
	free(shared->fork_mutexes);
	pthread_mutex_destroy(shared->stdout_lock);
	free(shared->stdout_lock);
	free(shared);
	return (NULL);
}

void	*teardown_2(t_shared_data *shared, int i)
{
	printf("Mutex initialization failed\n");
	free(shared->fork_mutexes[i]);
	while (i-- > 0)
	{
		pthread_mutex_destroy(shared->fork_mutexes[i]);
		free(shared->fork_mutexes[i]);
	}
	free(shared->fork_mutexes);
	pthread_mutex_destroy(shared->stdout_lock);
	free(shared->stdout_lock);
	free(shared);
	return (NULL);
}
