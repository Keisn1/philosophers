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

int main() {
	thinking_msg(1);
	dying_msg(1, 100);
}
