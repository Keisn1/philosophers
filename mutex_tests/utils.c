#include "philo.h"

t_thread_data *init_threads_data(int num_threads, unsigned long long base_time, unsigned long long time_to_die) {
	t_thread_data *threads_data = malloc(sizeof(t_thread_data) * num_threads);
	t_params params = {base_time, time_to_die};

	pthread_mutex_t *data_mutex = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(data_mutex, NULL);

	t_protected *protected = malloc(sizeof(t_protected));
	protected->philo_died = false;
	protected->fork_available = true;
	protected->data_mutex = data_mutex;
	for (int i = 0; i < num_threads; i++) {
		threads_data[i].thread_num = i;
		threads_data[i].time_last_meal = 0;
		threads_data[i].protected = protected;
		threads_data[i].params = params;
	}
	return threads_data;
}
