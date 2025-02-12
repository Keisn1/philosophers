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
#include <stdio.h>


typedef struct s_params {
	unsigned long long base_time;
	unsigned long long time_to_die;
} t_params;

typedef struct s_shared_data {
	bool philo_died;
	pthread_mutex_t *stdout_lock;
	pthread_mutex_t **forks;
} t_shared_data;

typedef struct s_philo {
	int thread_num;
	unsigned long long time_last_meal;
	t_shared_data *shared;
	t_params params;
	pthread_mutex_t *l_fork;
	pthread_mutex_t *r_fork;
} t_philo;

t_philo *init_philos(int num_threads, unsigned long long base_time, unsigned long long time_to_die);
unsigned long long get_timestamp(unsigned long long base_time);

int write_so();
typedef struct s_thread_info {
	unsigned long long timestamp;
} t_thread_info;

void	ft_putnbr_ull_fd(unsigned long long nb, int fd);
size_t	ft_putstr_fd(char *s, int fd);
size_t	ft_putchar_fd(char c, int fd);
int	ft_atoi(const char *nptr);

char* validate(char** argv);
int parse(char* str, unsigned int *nbr);
#endif // PHILO_H
