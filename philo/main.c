#include "philo.h"
#include <limits.h>

void thinking_msg(int philo) {
	ft_putnbr_ull_fd(philo, STDOUT_FILENO);
	ft_putstr_fd(" is thinking\n", STDOUT_FILENO);
}

void dying_msg(int philo, unsigned long long timestamp) {
	ft_putnbr_ull_fd(timestamp, STDOUT_FILENO);
	ft_putstr_fd(" ", STDOUT_FILENO);
	ft_putnbr_ull_fd(philo, STDOUT_FILENO);
	ft_putstr_fd(" died\n", STDOUT_FILENO);

}

int main(int argc, char** argv) {

	(void)argc;
	unsigned long long timestamp = (unsigned long long)ft_atoi(argv[2]);

	thinking_msg(1);
	dying_msg(1, timestamp);
}
