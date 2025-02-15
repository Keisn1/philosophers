#ifndef PHILO_BONUS_H
#define PHILO_BONUS_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

typedef struct s_shared_data {
	sem_t *fork_pile;
	sem_t *stdout_lock;
	sem_t *dead_lock;
} t_shared_data;

typedef struct s_params
{
	unsigned long long	base_time;
	int					num_philos;
	unsigned long long	time_to_die;
	unsigned long long	time_to_eat;
	unsigned long long	time_to_sleep;
	unsigned long long	must_eat;
}						t_params;

t_params	get_params(int argc, char **argv);

typedef struct s_philo_data {
	int philo_num;
	unsigned long long last_meal;
	t_shared_data shared;
	t_params params;
} t_philo_data;

void	check_args(int argc, char **argv);
int	parse(char *str, unsigned long long *nbr);
char	*validate(char **argv);
int	get_num_philos(char *argument);
unsigned long long	get_ull(char *argument);
unsigned long long	get_timestamp(void);

void print_fork_msg(t_philo_data *philo_d);
void print_eat_msg(t_philo_data *philo_d);
void print_sleep_msg(t_philo_data *philo_d);
void print_thinking_msg(t_philo_data *philo_d);
#endif // PHILO_BONUS_H
