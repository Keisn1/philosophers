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
#include <pthread.h>

typedef struct s_meal_check {
	t_shared_data shared;
	t_params params;
} t_meal_check;

void *meal_check_routine(void *params) {
	t_meal_check *meal_check = (t_meal_check*)params;

	int count = 0;
	while (count < meal_check->params.num_philos) {
		sem_wait(meal_check->shared.meal_sem);
		count++;
	}
	sem_wait(meal_check->shared.stdout_lock);
	printf("%llu: We have eaten enough\n", get_timestamp() - meal_check->params.base_time);
	sem_post(meal_check->shared.dead_lock);
	return NULL;
}

void *observer_routine_time(void *params) {
	t_philo_data *philo_d = (t_philo_data*)params;
	while (1) {
		sem_wait(philo_d->shared.check_lock);
		unsigned long long time = get_timestamp();
		if (time < philo_d->last_meal) {
			sem_post(philo_d->shared.check_lock);
			usleep(100);
			continue;
		}
		if (time - philo_d->last_meal >= philo_d->params.time_to_die) {
			sem_wait(philo_d->shared.stdout_lock);
			printf("%lld %d died\n", time - philo_d->params.base_time, philo_d->philo_num);
			sem_post(philo_d->shared.dead_lock);
			return NULL;
		}
		sem_post(philo_d->shared.check_lock);
		usleep(100);
	}
	return NULL;
}

void philo_routine(t_philo_data *philo_d) {
	pthread_t sub_thread;

	pthread_create(&sub_thread, NULL, observer_routine_time, philo_d);

	while (get_timestamp() < philo_d->params.base_time) {
		usleep(100);
	}
	while (1) {
		/* eating */
		sem_wait(philo_d->shared.fork_lock);
		sem_wait(philo_d->shared.fork_pile);
		print_fork_msg(philo_d);
		sem_wait(philo_d->shared.fork_pile);
		print_fork_msg(philo_d);
		sem_post(philo_d->shared.fork_lock);

		print_eat_msg(philo_d);
		while ((get_timestamp() - philo_d->last_meal) < philo_d->params.time_to_eat) {
			usleep(100);
		}
		philo_d->meals_eaten++;
		if (philo_d->meals_eaten == philo_d->params.must_eat) {
			sem_post(philo_d->shared.meal_sem);
		}
		sem_post(philo_d->shared.fork_pile);
		sem_post(philo_d->shared.fork_pile);

		/* sleeping */
		print_sleep_msg(philo_d);

		while ((get_timestamp() - philo_d->last_meal) < (philo_d->params.time_to_eat + philo_d->params.time_to_sleep)) {
			usleep(100);
		}

		/* thinking */
		print_thinking_msg(philo_d);

		if ((philo_d->params.time_to_die - (get_timestamp() - philo_d->last_meal)) > 10)
			usleep(1000);
	}
}



int	main(int argc, char **argv)
{
	t_params	params;
	t_shared_data shared;

	unlink_semaphores();
	check_args(argc, argv);
	params = get_params(argc, argv);
	set_semaphores(&shared , params.num_philos);

	pid_t *pids = malloc(sizeof(pid_t) * params.num_philos);

	int philo_num = 1;
	while (philo_num < params.num_philos+1) {
		pid_t pid = fork();
		if (pid < 0) {
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0) {
			t_philo_data philo_d = {philo_num, params.base_time, 0, shared, params};
			philo_routine(&philo_d);
			exit(EXIT_SUCCESS);
		}
		pids[philo_num-1] = pid;
		philo_num++;
	}

	t_meal_check meal_check = {shared, params};
	pthread_t meal_check_thread;
	pthread_create(&meal_check_thread, NULL, meal_check_routine, &meal_check);
	pthread_detach(meal_check_thread);
	sem_wait(shared.dead_lock);

	philo_num = 1;
	while (philo_num < params.num_philos+1) {
		kill(pids[philo_num-1], SIGINT);
		philo_num++;
	}
	free(pids);

	int status;
	int error;
	error =  waitpid(0 , &status, 0);
	/* perror("waitpid"); */
	/* printf("error: %d\n", error); */
	/* if (WIFSIGNALED(status)) { */
	/* 	printf("Was terminated by a signal: %d\n", WTERMSIG(status)); */
	/* } else { */
	/* 	printf("Not what you wanted\n"); */
	/* } */
	/* printf("at the end\n"); */
	sem_close(shared.fork_lock);
	sem_unlink(FORK_LOCK_SEM);
	sem_close(shared.fork_pile);
	sem_unlink(FORK_PILE_SEM);
	sem_close(shared.stdout_lock);
	sem_unlink(STDOUT_LOCK_SEM);
	sem_close(shared.dead_lock);
	sem_unlink(DEAD_LOCK_SEM);
	sem_close(shared.check_lock);
	sem_unlink(CHECK_LOCK_SEM);
	sem_close(shared.meal_sem);
	sem_unlink(MEAL_SEM);
	return (0);
}
