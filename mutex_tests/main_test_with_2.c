/* test with 2 philosophers where one of them shall die in about 100 ms only one fork */
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

unsigned long long get_timestamp(unsigned long long base_time) {
	struct timeval tv;
	if (gettimeofday(&tv, NULL))
		return -1;
	return ((unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000) - base_time;
}

void exit_perror(char *err_msg) {
	perror(err_msg);
	exit(EXIT_FAILURE);
}

typedef struct s_params {
	unsigned long long base_time;
	unsigned long long time_to_die;
} t_params;

typedef struct s_protected {
	bool philo_died;
	bool fork_available;
	pthread_mutex_t *data_mutex;
	pthread_mutex_t *fork;

} t_protected;

typedef struct s_thread_data {
	int thread_num;
	unsigned long long time_last_meal;
	t_protected *protected;
	t_params params;
} t_thread_data;


void *philo_loop(void *param) {
	t_thread_data *thread_data = (t_thread_data*)param;

	while (1) {
		pthread_mutex_lock(thread_data->protected->data_mutex);
		if (thread_data->protected->fork_available)
		{
			/* lock the fork and unlock data_mutex */
			pthread_mutex_lock(thread_data->protected->fork);
			printf("Thread %d got fork\n", thread_data->thread_num);
			thread_data->protected->fork_available = false;
			pthread_mutex_unlock(thread_data->protected->data_mutex);

			int count2 = 0;
			bool philo_dead = false;
			while (count2 < 100 && !philo_dead) {
				usleep(1 * 1000);
				count2++;
				pthread_mutex_lock(thread_data->protected->data_mutex);
				philo_dead = thread_data->protected->philo_died;
				pthread_mutex_unlock(thread_data->protected->data_mutex);
			}
			if (philo_dead)
				break;

			pthread_mutex_lock(thread_data->protected->data_mutex);
			pthread_mutex_unlock(thread_data->protected->fork);
			printf("Thread %d put down fork\n", thread_data->thread_num);
			thread_data->protected->fork_available = true;
			pthread_mutex_unlock(thread_data->protected->data_mutex);
		} else {
			if (thread_data->protected->philo_died == true) {
				pthread_mutex_unlock(thread_data->protected->data_mutex);
				break;
			}

			unsigned long long time = get_timestamp(thread_data->params.base_time);
			if (time - thread_data->time_last_meal > thread_data->params.time_to_die) {
				thread_data->protected->philo_died = true;
				printf("I'm dead: %d : %lld\n", thread_data->thread_num, time);
				pthread_mutex_unlock(thread_data->protected->data_mutex);
				break;
			}

			pthread_mutex_unlock(thread_data->protected->data_mutex);
			usleep(1 * 1000);
		}
	}
	return 0;
}

t_thread_data *init_threads_data(int num_threads, unsigned long long base_time, unsigned long long time_to_die) {
	t_thread_data *threads_data = malloc(sizeof(t_thread_data) * num_threads);
	t_params params = {base_time, time_to_die};


	pthread_mutex_t *fork = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(fork, NULL);

	pthread_mutex_t *data_mutex = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(data_mutex, NULL);

	t_protected *protected = malloc(sizeof(t_protected));
	protected->philo_died = false;
	protected->fork_available = true;
	protected->data_mutex = data_mutex;
	protected->fork	 = fork;
	for (int i = 0; i < num_threads; i++) {
		threads_data[i].thread_num = i;
		threads_data[i].time_last_meal = 0;
		threads_data[i].protected = protected;
		threads_data[i].params = params;
	}
	return threads_data;
}

int main() {
	int num_threads = 200;
	unsigned long long base_time = get_timestamp(0);
	unsigned long long time_to_die = 50;

	pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
	if (!threads)
		exit_perror("malloc");

	t_thread_data *thread_data = init_threads_data(num_threads, base_time, time_to_die);
	for (int i = 0; i < num_threads; ++i)
		pthread_create(&threads[i], NULL, philo_loop, &thread_data[i]);

	for (int i = 0; i < num_threads; ++i)
		pthread_join(threads[i], NULL);

	pthread_mutex_destroy(thread_data[0].protected->data_mutex);
	pthread_mutex_destroy(thread_data[0].protected->data_mutex);
	free(threads);
	free(thread_data);
	return 0;
}
