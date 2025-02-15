/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   params.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 14:29/23 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/15 14:29:23 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
	params.base_time = get_timestamp() + 500;
	return (params);
}
