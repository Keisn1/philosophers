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

void set_semaphores(t_shared_data *shared, int num_philos) {

	shared->fork_lock = sem_open(FORK_LOCK_SEM, O_CREAT, 0644, 1);
	if (shared->fork_pile == SEM_FAILED) {
		perror("sem_open failed");
		exit(EXIT_FAILURE);
	}
	shared->fork_pile = sem_open(FORK_PILE_SEM, O_CREAT, 0644, num_philos);
	if (shared->fork_pile == SEM_FAILED) {
		perror("sem_open failed");
		exit(EXIT_FAILURE);
	}
	shared->stdout_lock = sem_open(STDOUT_LOCK_SEM, O_CREAT, 0644, 1);
	if (shared->stdout_lock == SEM_FAILED) {
		perror("sem_open failed");
		exit(EXIT_FAILURE);
	}
	shared->dead_lock = sem_open(DEAD_LOCK_SEM, O_CREAT, 0644, 0);
	if (shared->dead_lock == SEM_FAILED) {
		perror("sem_open failed");
		exit(EXIT_FAILURE);
	}
	shared->check_lock = sem_open(CHECK_LOCK_SEM, O_CREAT, 0644, 1);
	if (shared->check_lock == SEM_FAILED) {
		perror("sem_open failed");
		exit(EXIT_FAILURE);
	}
	shared->meal_sem = sem_open(MEAL_SEM, O_CREAT, 0644, 0);
	if (shared->meal_sem == SEM_FAILED) {
		perror("sem_open failed");
		exit(EXIT_FAILURE);
	}
}

void unlink_semaphores() {
	sem_unlink(FORK_LOCK_SEM);
	sem_unlink(FORK_PILE_SEM);
	sem_unlink(STDOUT_LOCK_SEM);
	sem_unlink(DEAD_LOCK_SEM);
	sem_unlink(CHECK_LOCK_SEM);
	sem_unlink(MEAL_SEM);
}
