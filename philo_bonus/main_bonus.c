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



/* void observer_routine() { */
/* 	unsigned long long time_to_die = 200; */
/* 	while (1) { */

/* 		sem_wait(check_lock); */
/* 		unsigned long long time_since_last_meal = shared->time_since_last_meal; */
/* 		unsigned long long time = get_timestamp(); */
/* 		if (time_since_last_meal >= params.time_to_die) */
/* 			sem_wait(stdout_lock); */
/* 			printf("%lld %d died\n", time - observer->params.base_time, idx + 1); */
/* 			sem_post(dead_lock); */
/* 		} */
/* 		sem_post(check_lock); */
/* 	} */
/* } */

void philo_routine(t_philo_data *philo_d) {
	pthread_t sub_thread;

	pthread_create(&sub_thread, NULL, observer_routine, philo_d);

	while (1) {
		/* eating */
		sem_wait(philo_d->shared.fork_pile);
		print_fork_msg(philo_d);
		sem_wait(philo_d->shared.fork_pile);
		print_fork_msg(philo_d);

		print_eat_msg(philo_d);
		usleep(philo_d->params.time_to_eat * 1000);
		sem_post(philo_d->shared.fork_pile);
		sem_post(philo_d->shared.fork_pile);

		/* sleeping */
		print_sleep_msg(philo_d);
		usleep(philo_d->params.time_to_sleep * 1000);

		/* thinking */
		print_thinking_msg(philo_d);
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
