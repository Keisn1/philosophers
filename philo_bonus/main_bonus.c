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
#include <semaphore.h>

pid_t	*launch_philos(t_params params, t_shared_data shared)
{
	pid_t			*pids;
	int				philo_num;
	pid_t			pid;
	t_philo_data	philo_d;

	pids = malloc(sizeof(pid_t) * params.num_philos);
	if (!pids)
		perror_exit("malloc");
	philo_num = 1;
	while (philo_num < params.num_philos + 1)
	{
		pid = fork();
		if (pid < 0)
			perror_exit("fork");
		if (pid == 0)
		{
			philo_d = (t_philo_data){philo_num, params.base_time, 0, shared,
				params};
			philo_routine(&philo_d);
			exit(EXIT_SUCCESS);
		}
		pids[philo_num - 1] = pid;
		philo_num++;
	}
	return (pids);
}

void	kill_philos(t_params params, pid_t *pids)
{
	int	philo_num;
	int	error;
	int	status;

	philo_num = 1;
	while (philo_num < params.num_philos + 1)
	{
		kill(pids[philo_num - 1], SIGKILL);
		philo_num++;
	}
	free(pids);
	error = waitpid(0, &status, 0);
	if (error == -1)
	{
		perror("waitpid");
		exit(EXIT_FAILURE);
	}
	if (!WIFSIGNALED(status))
		exit_with_msg("Not terminated by Signal");
	if (!(WTERMSIG(status) == SIGKILL))
		exit_with_msg("Not terminated by SIGKILL");
}

void	simulation(t_params params)
{
	t_shared_data	shared;
	pid_t			*pids;
	t_meal_check	meal_check;
	pthread_t		meal_check_thread;

	set_semaphores(&shared, params.num_philos);
	pids = launch_philos(params, shared);
	meal_check = (t_meal_check){shared, params, false};
	pthread_create(&meal_check_thread, NULL, meal_check_routine, &meal_check);
	sem_wait(shared.dead_lock);
	sem_wait(shared.meal_stop_sem);
	meal_check.stop_meal_check = true;
	sem_post(shared.meal_stop_sem);
	sem_post(meal_check.shared.meal_sem);
	pthread_join(meal_check_thread, NULL);
	kill_philos(params, pids);
	unlink_and_close(shared);
}

int	main(int argc, char **argv)
{
	t_params	params;

	unlink_semaphores();
	check_args(argc, argv);
	params = get_params(argc, argv);
	simulation(params);
	return (0);
}
