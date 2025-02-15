/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argument_checks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 09:36/17 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/15 09:36:17 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_args(int argc, char **argv)
{
	char	*err_msg;

	if (argc < 5)
	{
		printf("too few arguments");
		exit(EXIT_FAILURE);
	}
	if (argc < 5 || argc > 6)
	{
		printf("too many arguments");
		exit(EXIT_FAILURE);
	}
	err_msg = validate(argv + 1);
	if (err_msg)
	{
		printf("%s\n", err_msg);
		exit(EXIT_FAILURE);
	}
}

unsigned long long	get_ull(char *argument)
{
	unsigned long long	time;

	if (parse(argument, &time))
	{
		printf("%s does not fit into unsigned long long\n", argument);
		exit(EXIT_FAILURE);
	}
	return (time);
}

int	get_num_philos(char *argument)
{
	unsigned long long	num_philos;

	num_philos = get_ull(argument);
	if (num_philos < 1)
	{
		printf("Need to have at least one philosopher\n");
		exit(EXIT_FAILURE);
	}
	if (num_philos > INT_MAX)
	{
		printf("Too many philos\n");
		exit(EXIT_FAILURE);
	}
	return ((int)num_philos);
}
