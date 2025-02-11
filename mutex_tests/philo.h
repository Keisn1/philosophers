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

typedef struct s_protected {
	bool philo_died;
	bool fork_available;
	pthread_mutex_t *data_mutex;
} t_protected;

typedef struct s_thread_data {
	int thread_num;
	unsigned long long time_last_meal;
	t_protected *protected;
	t_params params;
} t_thread_data;

t_thread_data *init_threads_data(int num_threads, unsigned long long base_time, unsigned long long time_to_die);

#endif // PHILO_H
