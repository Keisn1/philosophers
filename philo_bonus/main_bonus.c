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

#define FORK_PILE_SEM "/philo_fork_pile_sem"
#define STDOUT_LOCK_SEM "/philo_stdout_lock_sem"

typedef struct s_shared_data {
	sem_t *fork_pile;
	sem_t *stdout_lock;
	sem_t *dead_lock;
} t_shared_data;

void print_fork_msg(t_shared_data shared, t_params params, int philo_num) {
	sem_wait(shared.stdout_lock);
	printf("%lld %d has taken a fork\n", get_timestamp() - params.base_time, philo_num);
	sem_post(shared.stdout_lock);
}

void philo_routine(t_shared_data shared, int philo_num, t_params params) {
	bool philo_died;
	int num_forks = 0;

	philo_died = false;
	while (!philo_died) {
		while (num_forks != 2) {
			sem_wait(shared.fork_pile);
			print_fork_msg(shared, params, philo_num);
			sem_wait(shared.fork_pile);
			print_fork_msg(shared, params, philo_num);
		}


		sem_post(shared.fork_pile);
		sem_post(shared.fork_pile);
	}
}

int	main(int argc, char **argv)
{
	t_params	params;
	t_shared_data shared;

	check_args(argc, argv);
	params = get_params(argc, argv);


	shared.fork_pile = sem_open(FORK_PILE_SEM, O_CREAT, 0644, params.num_philos);
	if (shared.fork_pile == SEM_FAILED) {
		perror("sem_open failed");
		exit(EXIT_FAILURE);
	}
	shared.stdout_lock = sem_open(STDOUT_LOCK_SEM, O_CREAT, 0644, params.num_philos);
	if (shared.stdout_lock == SEM_FAILED) {
		perror("sem_open failed");
		exit(EXIT_FAILURE);
	}
	shared.dead_lock = sem_open(STDOUT_LOCK_SEM, O_CREAT, 0644, params.num_philos);
	if (shared.dead_lock == SEM_FAILED) {
		perror("sem_open failed");
		exit(EXIT_FAILURE);
	}


	sem_wait(shared.dead_lock);


	sem_close(shared.fork_pile);
	sem_unlink(FORK_PILE_SEM);
	sem_close(shared.stdout_lock);
	sem_unlink(FORK_PILE_SEM);
	return (0);
}
