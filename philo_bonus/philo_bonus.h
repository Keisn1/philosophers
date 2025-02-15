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

void	check_args(int argc, char **argv);
int	parse(char *str, unsigned long long *nbr);
char	*validate(char **argv);
int	get_num_philos(char *argument);
unsigned long long	get_ull(char *argument);
unsigned long long	get_timestamp(void);

#endif // PHILO_BONUS_H
