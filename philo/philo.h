#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <unistd.h>
#include <stddef.h>

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
