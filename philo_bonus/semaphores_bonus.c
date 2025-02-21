/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphores.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 10:14/38 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/16 10:14:38 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	sem_error(void)
{
	perror("sem_open failed");
	exit(EXIT_FAILURE);
}

void	set_semaphores(t_shared_data *shared, int num_philos)
{
	shared->fork_lock = sem_open(FORK_LOCK_SEM, O_CREAT, 0644, 1);
	if (shared->fork_lock == SEM_FAILED)
		sem_error();
	shared->fork_pile = sem_open(FORK_PILE_SEM, O_CREAT, 0644, num_philos);
	if (shared->fork_pile == SEM_FAILED)
		sem_error();
	shared->stdout_lock = sem_open(STDOUT_LOCK_SEM, O_CREAT, 0644, 1);
	if (shared->stdout_lock == SEM_FAILED)
		sem_error();
	shared->dead_lock = sem_open(DEAD_LOCK_SEM, O_CREAT, 0644, 0);
	if (shared->dead_lock == SEM_FAILED)
		sem_error();
	shared->check_lock = sem_open(CHECK_LOCK_SEM, O_CREAT, 0644, 1);
	if (shared->check_lock == SEM_FAILED)
		sem_error();
	shared->meal_sem = sem_open(MEAL_SEM, O_CREAT, 0644, 0);
	if (shared->meal_sem == SEM_FAILED)
		sem_error();
	shared->meal_stop_sem = sem_open(MEAL_STOP_SEM, O_CREAT, 0644, 0);
	if (shared->meal_stop_sem == SEM_FAILED)
		sem_error();
}

void	unlink_semaphores(void)
{
	sem_unlink(FORK_LOCK_SEM);
	sem_unlink(FORK_PILE_SEM);
	sem_unlink(STDOUT_LOCK_SEM);
	sem_unlink(DEAD_LOCK_SEM);
	sem_unlink(CHECK_LOCK_SEM);
	sem_unlink(MEAL_SEM);
	sem_unlink(MEAL_STOP_SEM);
}

void	unlink_and_close(t_shared_data shared)
{
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
	sem_close(shared.meal_stop_sem);
	sem_unlink(MEAL_STOP_SEM);
}
