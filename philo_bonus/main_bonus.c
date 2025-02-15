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

#include "philo_bonus.h"
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#define FORK_PILE_SEM "/philo_fork_pile"

typedef struct s_shared_data {
	sem_t *fork_pile;
} t_shared_data;

void philo_routine(t_shared_data shared) {
	bool philo_died;
	int num_forks = 0;

	philo_died = false;
	while (!philo_died) {
		while (num_forks != 2) {
			sem_wait(shared.fork_pile);
			sem_wait(shared.fork_pile);
		}


		sem_wait(shared.fork_pile);
		sem_wait(shared.fork_pile);
	}
}

int	main(int argc, char **argv)
{
	t_params	params;
	t_shared_data shared;

	check_args(argc, argv);
	params = get_params(argc, argv);


	shared.fork_pile = sem_open(FORK_PILE_SEM, O_CREAT, 0644, 1);
	if (shared.fork_pile == SEM_FAILED) {
		perror("sem_open failed");
		exit(EXIT_FAILURE);
	}


	sem_close(shared.fork_pile);
	sem_unlink(FORK_PILE_SEM);

	return (0);
}
