#include "philo.h"
#include <pthread.h>

void *teardown_1(pthread_mutex_t **forks, int i, pthread_mutex_t *stdout_lock, t_shared_data *shared)
{
	printf("Allocation failed\n");
	while (i-- > 0) {
		pthread_mutex_destroy(forks[i]);
		free(forks[i]);
	}
	free(forks);
	pthread_mutex_destroy(stdout_lock);
	free(stdout_lock);
	free(shared);
	return NULL;
}

void *teardown_2(pthread_mutex_t **forks, int i, pthread_mutex_t *stdout_lock, t_shared_data *shared)
{
	printf("Mutex initialization failed\n");
	free(forks[i]);
	while (i-- > 0) {
		pthread_mutex_destroy(forks[i]);
		free(forks[i]);
	}
	free(forks);
	pthread_mutex_destroy(stdout_lock);
	free(stdout_lock);
	free(shared);
	return NULL;
}

void *teardown_3(pthread_mutex_t *stdout_lock, t_shared_data *shared)
{
	printf("Allocation failed\n");
	pthread_mutex_destroy(stdout_lock);
	free(stdout_lock);
	free(shared);
	return NULL;
}

void *teardown_4(pthread_mutex_t *stdout_lock, t_shared_data *shared) {
	printf("Mutex initialization failed\n");
	free(stdout_lock);
	free(shared);
	return NULL;
}

void *teardown_5(t_shared_data *shared) {
	printf("Allocation failed\n");
	free(shared);
	return NULL;
}

void *teardown_6() {
	printf("Allocation failed\n");
	return NULL;
}

t_shared_data *set_shared_data(t_shared_data *shared, pthread_mutex_t **forks, int num_threads, pthread_mutex_t *stdout_lock) {
	int i = 0;
	while (i < num_threads)
		{
		forks[i] = malloc(sizeof(pthread_mutex_t));
		if (!forks[i])
			return teardown_1(forks, i, stdout_lock, shared);
		if (pthread_mutex_init(forks[i], NULL))
			return teardown_2(forks, i, stdout_lock, shared);
		i++;
	}
	shared->philo_died = false;
	shared->stdout_lock = stdout_lock;
	return shared;
}

t_shared_data *init_shared_data(int num_threads) {
	t_shared_data *shared = malloc(sizeof(t_shared_data));
	if (!shared)
		return teardown_6();

	pthread_mutex_t *stdout_lock = malloc(sizeof(pthread_mutex_t));
	if (!stdout_lock)
		return teardown_5(shared);

	if (pthread_mutex_init(stdout_lock, NULL))
		return teardown_4(stdout_lock, shared);

	pthread_mutex_t **forks = malloc(sizeof(pthread_mutex_t*) * num_threads);
	if (!forks)
		return teardown_3(stdout_lock, shared);
	shared->forks = forks;

	return set_shared_data(shared, forks, num_threads, stdout_lock);
}

t_philo *init_philos(int num_threads, unsigned long long base_time, unsigned long long time_to_die) {
	t_philo *philos = malloc(sizeof(t_philo) * num_threads);
	if (!philos)
		return NULL;

	t_shared_data *shared = init_shared_data(num_threads);
	if (!shared) {
		free(philos);
		return NULL;
	}
	t_params params = {base_time, time_to_die};
	int i = 0;
	while (i < num_threads) {
		philos[i].thread_num = i;
		philos[i].time_last_meal = 0;
		philos[i].shared = shared;
		philos[i++].params = params;
	}

	i = 0;
	pthread_mutex_t **forks = shared->forks;
	while (i < num_threads-1) {
		philos[i].r_fork = forks[i];
		philos[i].l_fork = forks[i+1];
		i++;
	}
	philos[num_threads-1].r_fork = forks[num_threads-1];
	philos[num_threads-1].l_fork = forks[0];

	return philos;
}
