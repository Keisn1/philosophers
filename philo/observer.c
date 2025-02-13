#include "philo.h"

void *observer_routine(void *params) {
	t_observer *observer = (t_observer*)params;
	bool philo_dead = false;
	while (!philo_dead) {
		int i = 0;
		pthread_mutex_lock(observer->shared->check_lock);
		while (i < observer->num_philos) {
			if ((get_timestamp() - observer->philos[i].time_last_meal) > observer->params.time_to_die) {
				observer->shared->philo_died = true;
				philo_dead = true;
				pthread_mutex_lock(observer->shared->stdout_lock);
				printf("%lld %d died\n", get_timestamp() - observer->params.base_time, i+1);
				pthread_mutex_unlock(observer->shared->stdout_lock);
				break;
			}
			i++;
		}
		pthread_mutex_unlock(observer->shared->check_lock);
		if (philo_dead)
			break;
		usleep(100);
	}
	return NULL;
}
