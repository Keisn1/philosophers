#include "philo.h"

void teardown(t_philo *philos, int num_philos, pthread_t *threads)
{
	int i = 0;
	pthread_mutex_t **all_forks = philos[0].shared->forks;
	while (i < num_philos) {
		pthread_mutex_destroy(all_forks[i]);
		free(all_forks[i++]);
	}
	free(all_forks);
	pthread_mutex_destroy(philos[0].shared->stdout_lock);
	free(philos[0].shared->stdout_lock);
	free(philos[0].shared);
	free(philos);
	free(threads);

}
