#include "philo.h"

bool try_to_get_r_fork(t_philo *philo) {
	bool got_r_fork = false;
	pthread_mutex_lock(philo->r_fork_mutex);
	if (*(philo->r_fork) == false) {
		*(philo->r_fork) = true;
		got_r_fork = true;
	}
	pthread_mutex_unlock(philo->r_fork_mutex);
	return got_r_fork;
}

bool try_to_get_l_fork(t_philo *philo) {
	bool got_l_fork = false;
	pthread_mutex_lock(philo->l_fork_mutex);
	if (*(philo->l_fork) == false) {
		*(philo->l_fork) = true;
		got_l_fork = true;
	}
	pthread_mutex_unlock(philo->l_fork_mutex);
	return got_l_fork;
}

void give_up_forks(t_philo *philo) {
	pthread_mutex_lock(philo->l_fork_mutex);
	*(philo->l_fork) = false;
	pthread_mutex_unlock(philo->l_fork_mutex);

	pthread_mutex_lock(philo->r_fork_mutex);
	*(philo->r_fork) = false;
	pthread_mutex_unlock(philo->r_fork_mutex);
}
