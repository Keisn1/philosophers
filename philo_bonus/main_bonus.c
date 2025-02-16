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
	if (error != SIGINT) {
		perror("waitpid");
		exit(EXIT_FAILURE);
	}

	return (0);
}
