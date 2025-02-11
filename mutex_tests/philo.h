#ifndef PHILO_H
#define PHILO_H

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct s_params {
	unsigned long long base_time;
	unsigned long long time_to_die;
} t_params;

typedef struct s_shared_data {
	bool philo_died;
	pthread_mutex_t *stdout_lock;
} t_shared_data;

typedef struct s_philo {
	int thread_num;
	unsigned long long time_last_meal;
	t_shared_data *shared;
	t_params params;
	pthread_mutex_t *l_fork;
	pthread_mutex_t *r_fork;
} t_philo;

t_philo *init_threads_data(int num_threads, unsigned long long base_time, unsigned long long time_to_die);

#endif // PHILO_H
