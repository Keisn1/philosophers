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
	unsigned long long must_eat;
} t_params;

typedef struct s_shared_data {
	bool philo_died;
	bool *forks;
	pthread_mutex_t **fork_mutexes;
	pthread_mutex_t *stdout_lock;
	pthread_mutex_t *check_lock;
} t_shared_data;

typedef struct s_philo {
	int philo_num;
	unsigned long long time_last_meal;
	unsigned long long meals_eaten;
	int ate_enough;
	t_shared_data *shared;
	t_params params;
	bool *l_fork;
	bool *r_fork;
	pthread_mutex_t *l_fork_mutex;
	pthread_mutex_t *r_fork_mutex;
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


void print_fork_msg(t_philo *philo);
void print_sleep_msg(t_philo *philo);
void print_thinking_msg(t_philo *philo);

bool try_to_get_r_fork(t_philo *philo);
bool try_to_get_l_fork(t_philo *philo);
void give_up_forks(t_philo *philo);

void *observer_routine(void *params);
void check_args(int argc, char** argv);

unsigned long long get_num_philos(char *argument);
unsigned long long get_ull(char* argument);

#endif // PHILO_H
