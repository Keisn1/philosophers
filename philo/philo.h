/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 10:31/34 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/15 10:31:34 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_params
{
	unsigned long long	base_time;
	int					num_philos;
	unsigned long long	time_to_die;
	unsigned long long	time_to_eat;
	unsigned long long	time_to_sleep;
	unsigned long long	must_eat;
}						t_params;

typedef struct s_shared_data
{
	bool				philo_died;
	bool				*forks;
	pthread_mutex_t		**fork_mutexes;
	pthread_mutex_t		*stdout_lock;
	pthread_mutex_t		*check_lock;
}						t_shared_data;

typedef struct s_philo
{
	int					philo_num;
	unsigned long long	time_last_meal;
	unsigned long long	meals_eaten;
	int					ate_enough;
	t_shared_data		*shared;
	t_params			params;
	bool				*l_fork;
	bool				*r_fork;
	pthread_mutex_t		*l_fork_mutex;
	pthread_mutex_t		*r_fork_mutex;
	pthread_t			thread;
}						t_philo;

typedef struct s_observer
{
	t_philo				*philos;
	t_shared_data		*shared;
	t_params			params;
}						t_observer;

t_philo					*init_philos(int num_philos, t_params params);
unsigned long long		get_timestamp(void);

void					teardown_main(t_philo *philos, pthread_t *threads);
void					*teardown_1(t_shared_data *shared, int i);
void					*teardown_2(t_shared_data *shared, int i);
void					*teardown_3(pthread_mutex_t *stdout_lock,
							t_shared_data *shared);
void					*teardown_4(pthread_mutex_t *stdout_lock,
							t_shared_data *shared);
void					*teardown_5(t_shared_data *shared);
void					*teardown_6(void);

int						write_so(void);
typedef struct s_thread_info
{
	unsigned long long	timestamp;
}						t_thread_info;

char					*validate(char **argv);
int						parse(char *str, unsigned long long *nbr);

void					print_fork_msg(t_philo *philo);
void					print_sleep_msg(t_philo *philo);
void					print_thinking_msg(t_philo *philo);
void					print_eat_msg(t_philo *philo);
void					print_philo_died(t_observer *observer, int idx,
							unsigned long long time);

bool					try_to_get_r_fork(t_philo *philo);
bool					try_to_get_l_fork(t_philo *philo);
void					give_up_forks(t_philo *philo);

void					*observer_routine(void *params);
void					*philo_routine(void *params);
void					check_args(int argc, char **argv);

int						get_num_philos(char *argument);
unsigned long long		get_ull(char *argument);

bool					check_philo_died(t_philo *philo);
void					set_last_meal(t_philo *philo, unsigned long long start);
bool					sleep_loop(t_philo *philo, unsigned long long start,
							unsigned long long time);
bool					sleeping(t_philo *philo);

t_shared_data			*init_shared_data(int num_philos);

void					wait_for_base_time(unsigned long long base_time);

t_observer				get_observer(t_philo *philos);
t_params				get_params(int argc, char **argv);
#endif // PHILO_H
