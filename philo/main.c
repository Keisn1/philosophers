/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 12:40/24 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/15 12:40:24 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

void	launch(t_philo *philos, pthread_t *philo_threads, t_observer *observer)
{
	int			i;
	pthread_t	observer_thread;

	i = 0;
	pthread_create(&observer_thread, NULL, observer_routine, observer);
	while (i < observer->params.num_philos)
	{
		pthread_create(&philo_threads[i], NULL, philo_routine, &philos[i]);
		i++;
	}
	pthread_join(observer_thread, NULL);
	i = 0;
	while (i < observer->params.num_philos)
	{
		pthread_join(philo_threads[i], NULL);
		i++;
	}
}

void	simulation(t_params params)
{
	t_philo		*philos;
	pthread_t	*philo_threads;
	t_observer	observer;

	philo_threads = malloc(sizeof(pthread_t) * params.num_philos);
	if (!philo_threads)
	{
		perror("error");
		exit(EXIT_FAILURE);
	}
	philos = init_philos(params.num_philos, params);
	observer = get_observer(philos);
	launch(philos, philo_threads, &observer);
	teardown_main(philos, philo_threads);
}

int	main(int argc, char **argv)
{
	t_params	params;

	check_args(argc, argv);
	params = get_params(argc, argv);
	simulation(params);
	return (0);
}
