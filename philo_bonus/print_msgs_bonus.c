/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_msgs_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 18:40/12 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/15 18:40:12 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void print_fork_msg(t_philo_data *philo_d) {
	sem_wait(philo_d->shared.check_lock);
	sem_wait(philo_d->shared.stdout_lock);
	printf("%lld %d has taken a fork\n", get_timestamp() - philo_d->params.base_time, philo_d->philo_num);
	sem_post(philo_d->shared.stdout_lock);
	sem_post(philo_d->shared.check_lock);
}

void print_eat_msg(t_philo_data *philo_d) {
	sem_wait(philo_d->shared.check_lock);
	unsigned long long time = get_timestamp();
	philo_d->last_meal = time;
	sem_wait(philo_d->shared.stdout_lock);
	printf("%lld %d is eating\n", time - philo_d->params.base_time, philo_d->philo_num);
	sem_post(philo_d->shared.stdout_lock);
	sem_post(philo_d->shared.check_lock);
}

void print_sleep_msg(t_philo_data *philo_d) {
	sem_wait(philo_d->shared.check_lock);
	sem_wait(philo_d->shared.stdout_lock);
	printf("%lld %d is sleeping\n", get_timestamp() - philo_d->params.base_time, philo_d->philo_num);
	sem_post(philo_d->shared.stdout_lock);
	sem_post(philo_d->shared.check_lock);
}

void print_thinking_msg(t_philo_data *philo_d) {
	sem_wait(philo_d->shared.check_lock);
	sem_wait(philo_d->shared.stdout_lock);
	printf("%lld %d is thinking\n", get_timestamp() - philo_d->params.base_time, philo_d->philo_num);
	sem_post(philo_d->shared.stdout_lock);
	sem_post(philo_d->shared.check_lock);
}
