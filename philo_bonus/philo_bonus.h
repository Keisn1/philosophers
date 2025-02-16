/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 10:16/23 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/16 10:16:23 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h> /* For O_* constants */
# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h> /* For mode constants */
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

# define FORK_LOCK_SEM "/philo_fork_lock_sem"
# define FORK_PILE_SEM "/philo_fork_pile_sem"
# define STDOUT_LOCK_SEM "/philo_stdout_lock_sem"
# define DEAD_LOCK_SEM "/philo_dead_lock_sem"
# define CHECK_LOCK_SEM "/philo_check_lock_sem"
# define MEAL_SEM "/philo_meal_sem"

typedef struct s_shared_data
{
	sem_t				*fork_lock;
	sem_t				*fork_pile;
	sem_t				*stdout_lock;
	sem_t				*dead_lock;
	sem_t				*check_lock;
	sem_t				*meal_sem;
}						t_shared_data;

typedef struct s_params
{
	unsigned long long	base_time;
	int					num_philos;
	unsigned long long	time_to_die;
	unsigned long long	time_to_eat;
	unsigned long long	time_to_sleep;
	unsigned long long	must_eat;
}						t_params;

t_params				get_params(int argc, char **argv);

typedef struct s_philo_data
{
	int					philo_num;
	unsigned long long	last_meal;
	unsigned			meals_eaten;
	t_shared_data		shared;
	t_params			params;
}						t_philo_data;

void					check_args(int argc, char **argv);
int						parse(char *str, unsigned long long *nbr);
char					*validate(char **argv);
int						get_num_philos(char *argument);
unsigned long long		get_ull(char *argument);
unsigned long long		get_timestamp(void);
void					set_semaphores(t_shared_data *shared, int num_philos);
void					unlink_semaphores(void);

void					print_fork_msg(t_philo_data *philo_d);
void					print_eat_msg(t_philo_data *philo_d);
void					print_sleep_msg(t_philo_data *philo_d);
void					print_thinking_msg(t_philo_data *philo_d);

#endif // PHILO_BONUS_H
