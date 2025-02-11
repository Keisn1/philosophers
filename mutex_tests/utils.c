#include "philo.h"

t_philo *init_threads_data(int num_threads, unsigned long long base_time, unsigned long long time_to_die) {
	t_philo *philos = malloc(sizeof(t_philo) * num_threads);
	if (!philos)
		return NULL;

	pthread_mutex_t *stdout_lock = malloc(sizeof(pthread_mutex_t));
	if (!stdout_lock) {
		free(philos);
		return NULL;
	}

	if (pthread_mutex_init(stdout_lock, NULL)) {
		printf("Error initializing stdout_lock");
		free(philos);
		free(stdout_lock);
		return NULL;
	}

	t_shared_data *shared = malloc(sizeof(t_shared_data));
	shared->philo_died = false;
	shared->stdout_lock = stdout_lock;

	t_params params = {base_time, time_to_die};
	int i = 0;
	while (i < num_threads) {
		philos[i].thread_num = i;
		philos[i].time_last_meal = 0;
		philos[i].shared = shared;
		philos[i].params = params;
		i++;
	}

	pthread_mutex_t *forks = malloc(sizeof(pthread_mutex_t) * num_threads);
	i = 0;
	while (i < num_threads) {
		pthread_mutex_init(&forks[i], NULL);
	}

	i = 0;
	while (i < num_threads-1) {
		philos[i].r_fork = &forks[i];
		philos[i].l_fork = &forks[i+1];
	}
	philos[num_threads-1].r_fork = &forks[num_threads-1];
	philos[num_threads-1].l_fork = &forks[0];

	return philos;
}
