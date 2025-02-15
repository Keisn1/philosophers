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

void	exit_perror(char *err_msg)
{
	perror(err_msg);
	exit(EXIT_FAILURE);
}

t_observer	get_observer(t_philo *philos)
{
	t_observer	observer;

	observer.philos = philos;
	observer.shared = philos[0].shared;
	observer.params = philos[0].params;
	return (observer);
}

void	check_values_params(t_params params)
{
	if (params.num_philos < 1)
	{
		printf("At least 1 philo\n");
		exit(EXIT_FAILURE);
	}
	if (params.time_to_die < 1)
	{
		printf("Time to die is 0\n");
		exit(EXIT_FAILURE);
	}
	if (params.time_to_eat < 1)
	{
		printf("Time to eat is 0\n");
		exit(EXIT_FAILURE);
	}
	if (params.time_to_sleep < 1)
	{
		printf("Time to sleep is 0\n");
		exit(EXIT_FAILURE);
	}
}

int	get_must_eat(int argc, char **argv)
{
	unsigned long long	must_eat;

	must_eat = 0;
	if (argc == 6)
	{
		must_eat = get_ull(argv[5]);
		if (must_eat == 0)
		{
			printf("Philosophers must eat!\n");
			exit(EXIT_FAILURE);
		}
	}
	if (must_eat > INT_MAX)
	{
		printf("Philosophers can't eat that much!\n");
		exit(EXIT_FAILURE);
	}
	else if (must_eat == 0)
		return (-1);
	else
		return ((int)must_eat);
}

t_params	get_params(int argc, char **argv)
{
	t_params	params;

	params.num_philos = get_num_philos(argv[1]);
	params.time_to_die = get_ull(argv[2]);
	params.time_to_eat = get_ull(argv[3]);
	params.time_to_sleep = get_ull(argv[4]);
	params.must_eat = get_must_eat(argc, argv);
	check_values_params(params);
	return (params);
}

int	main(int argc, char **argv)
{
	t_params	params;
	pthread_t	*philo_threads;
	pthread_t	observer_thread;
	t_philo		*philos;
	t_observer	observer;

	check_args(argc, argv);
	params = get_params(argc, argv);
	philo_threads = malloc(sizeof(pthread_t) * params.num_philos);
	if (!philo_threads)
		exit_perror("malloc");
	philos = init_philos(params.num_philos);
	params.base_time = get_timestamp();
	set_philo_params(philos, params);
	observer = get_observer(philos);
	pthread_create(&observer_thread, NULL, observer_routine, &observer);

	for (int i = 0; i < params.num_philos; ++i)
		pthread_create(&philo_threads[i], NULL, philo_routine, &philos[i]);
	pthread_join(observer_thread, NULL);
	for (int i = 0; i < params.num_philos; ++i)
		pthread_join(philo_threads[i], NULL);
	teardown_main(philos, philo_threads);
	return (0);
}
