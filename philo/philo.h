#ifndef PHILO_H
#define PHILO_H

#include <unistd.h>
#include <stddef.h>

void	ft_putnbr_ull_fd(unsigned long long nb, int fd);
size_t	ft_putstr_fd(char *s, int fd);
size_t	ft_putchar_fd(char c, int fd);
int	ft_atoi(const char *nptr);

#endif // PHILO_H
