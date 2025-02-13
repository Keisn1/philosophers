#include "philo.h"


void print_fork_msg(t_philo *philo) {
	pthread_mutex_lock(philo->shared->stdout_lock);
	printf("%lld %d has taken a fork\n", get_timestamp() - philo->params.base_time, philo->philo_num);
	pthread_mutex_unlock(philo->shared->stdout_lock);
}

void print_sleep_msg(t_philo *philo) {
	pthread_mutex_lock(philo->shared->stdout_lock);
	printf("%lld %d is sleeping\n", get_timestamp() - philo->params.base_time, philo->philo_num);
	pthread_mutex_unlock(philo->shared->stdout_lock);
}

void print_thinking_msg(t_philo *philo) {
	pthread_mutex_lock(philo->shared->stdout_lock);
	printf("%lld %d is thinking\n", get_timestamp() - philo->params.base_time, philo->philo_num);
	pthread_mutex_unlock(philo->shared->stdout_lock);
}
