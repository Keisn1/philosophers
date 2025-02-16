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
#include <stdlib.h>
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <sys/wait.h>

#define FORK_PILE_SEM "/philo_fork_pile_sem"
#define STDOUT_LOCK_SEM "/philo_stdout_lock_sem"
#define DEAD_LOCK_SEM "/philo_dead_lock_sem"
#define CHECK_LOCK_SEM "/philo_check_lock_sem"

void *observer_routine(void *params) {
	t_philo_data *philo_d = (t_philo_data*)params;
	while (1) {
		sem_wait(philo_d->shared.check_lock);
		unsigned long long time = get_timestamp();
		if (time < philo_d->last_meal) {
			sem_post(philo_d->shared.check_lock);
			usleep(100);
			continue;
		}
		printf("%llu\n", time - philo_d->last_meal);
		if (time - philo_d->last_meal >= philo_d->params.time_to_die) {
			sem_wait(philo_d->shared.stdout_lock);
			printf("%lld %d died\n", time - philo_d->params.base_time, philo_d->philo_num);
			sem_post(philo_d->shared.dead_lock);
			return NULL;
		}
		sem_post(philo_d->shared.check_lock);
		usleep(1000);
	}
	return NULL;
}

void philo_routine(t_philo_data *philo_d) {
	pthread_t sub_thread;

	/* unlink and close anything that has been there before */
	pthread_create(&sub_thread, NULL, observer_routine, philo_d);

	while (get_timestamp() < philo_d->params.base_time) {
		usleep(100);
	}
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
	shared.stdout_lock = sem_open(STDOUT_LOCK_SEM, O_CREAT, 0644, 1);
	if (shared.stdout_lock == SEM_FAILED) {
		perror("sem_open failed");
		exit(EXIT_FAILURE);
	}
	shared.dead_lock = sem_open(DEAD_LOCK_SEM, O_CREAT, 0644, 0);
	if (shared.dead_lock == SEM_FAILED) {
		perror("sem_open failed");
		exit(EXIT_FAILURE);
	}
	shared.check_lock = sem_open(CHECK_LOCK_SEM, O_CREAT, 0644, 1);
	if (shared.check_lock == SEM_FAILED) {
		perror("sem_open failed");
		exit(EXIT_FAILURE);
	}

	t_philo_data philo_d = {1, params.base_time, shared, params};

	pid_t pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0) {
		philo_routine(&philo_d);
		exit(EXIT_SUCCESS);
	}

	sem_wait(shared.dead_lock);
	printf("crosses sem_wait(dead_lock)\n");

	int status;
	if (waitpid(0 , &status, 0)) {
		perror("waitpid");
		exit(EXIT_FAILURE);
	}
	if (WIFSIGNALED(status)) {
		printf("Was terminated by a signal: %d\n", WTERMSIG(status));
	} else {
		printf("Not what you wanted\n");
	}

	sem_close(shared.fork_pile);
	sem_unlink(FORK_PILE_SEM);
	sem_close(shared.stdout_lock);
	sem_unlink(STDOUT_LOCK_SEM);
	sem_close(shared.dead_lock);
	sem_unlink(DEAD_LOCK_SEM);
	sem_close(shared.check_lock);
	sem_unlink(CHECK_LOCK_SEM);
	return (0);
}
