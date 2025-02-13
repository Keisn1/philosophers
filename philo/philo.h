#ifndef PHILO_H
#define PHILO_H

#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stddef.h>
#include <limits.h>


typedef struct s_params {
	unsigned long long base_time;
	unsigned long long time_to_die;
	unsigned long long time_to_eat;
	unsigned long long time_to_sleep;
} t_params;

typedef struct s_shared_data {
	bool philo_died;
	pthread_mutex_t **forks;
	pthread_mutex_t *stdout_lock;
	pthread_mutex_t *check_lock;
} t_shared_data;

typedef struct s_philo {
	int philo_num;
	unsigned long long time_last_meal;
	t_shared_data *shared;
	t_params params;
	pthread_mutex_t *l_fork;
	pthread_mutex_t *r_fork;
} t_philo;

typedef struct s_observer {
	int num_philos;
	t_philo *philos;
	t_shared_data *shared;
	t_params params;
} t_observer;

t_philo *init_philos(int num_philos);
void set_philo_params(t_philo *philos, int num_philos, t_params params);
unsigned long long get_timestamp();

void teardown(t_philo *philos, int num_philos, pthread_t *threads);
void	*teardown_1(pthread_mutex_t **forks, int i, pthread_mutex_t *stdout_lock, t_shared_data *shared);
void	*teardown_2(pthread_mutex_t **forks, int i, pthread_mutex_t *stdout_lock, t_shared_data *shared);
void	*teardown_3(pthread_mutex_t *stdout_lock, t_shared_data *shared);
void	*teardown_4(pthread_mutex_t *stdout_lock, t_shared_data *shared);
void	*teardown_5(t_shared_data *shared);
void	*teardown_6(void);

int write_so();
typedef struct s_thread_info {
	unsigned long long timestamp;
} t_thread_info;

char* validate(char** argv);
int parse(char* str, unsigned long long *nbr);
#endif // PHILO_H
