/* test with 2 philosophers where one of them shall die in about 100 ms only one fork */
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void exit_perror(char *err_msg) {
	perror(err_msg);
	exit(EXIT_FAILURE);
}

typedef struct s_common_data {
	bool philo_died;
	bool fork_available;
	unsigned long long base_time;
	unsigned long long time_to_die;
	pthread_mutex_t *data_mutex;
	pthread_mutex_t *fork;

} t_common_data;

typedef struct s_thread_data {
	int thread_num;
	unsigned long long time_since_last_meal;
	t_common_data *common_data;

} t_thread_data;




void *philo_loop(void *param) {
	t_thread_data *thread_data = (t_thread_data*)param;

	int count = 0;
	while (count < 50) {
		pthread_mutex_lock(thread_data->common_data->data_mutex);
		if (thread_data->common_data->fork_available)
		{
			/* lock the fork and unlock data_mutex */
			pthread_mutex_lock(thread_data->common_data->fork);
			printf("Thread %d got fork\n", thread_data->thread_num);
			thread_data->common_data->fork_available = false;
			pthread_mutex_unlock(thread_data->common_data->data_mutex);

			int count2 = 0;
			bool philo_dead = false;
			while (count2 < 100 && !philo_dead) {
				usleep(1 * 1000);
				count2++;
				pthread_mutex_lock(thread_data->common_data->data_mutex);
				philo_dead = thread_data->common_data->philo_died;
				pthread_mutex_unlock(thread_data->common_data->data_mutex);
			}
			if (philo_dead)
				break;

			pthread_mutex_lock(thread_data->common_data->data_mutex);
			pthread_mutex_unlock(thread_data->common_data->fork);
			printf("Thread %d put down fork\n", thread_data->thread_num);
			thread_data->common_data->fork_available = true;
			pthread_mutex_unlock(thread_data->common_data->data_mutex);
		} else {
			struct timeval tv;
			if (gettimeofday(&tv, NULL))
				exit(EXIT_FAILURE);

			if (thread_data->common_data->philo_died == true) {
				pthread_mutex_unlock(thread_data->common_data->data_mutex);
				break;
			}

			unsigned long long time = (unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000 - thread_data->common_data->base_time;
			printf("%lld\n", time);
			if (thread_data->time_since_last_meal - time > thread_data->common_data->time_to_die) {
				thread_data->common_data->philo_died = true;
				printf("I'm dead: %d\n", thread_data->thread_num);
				pthread_mutex_unlock(thread_data->common_data->data_mutex);
				break;
			}

			pthread_mutex_unlock(thread_data->common_data->data_mutex);
			usleep(1 * 1000);
			count++;
		}
	}
	pthread_mutex_lock(thread_data->common_data->data_mutex);
	if (count >= 50)
		printf("I'm dead: %d\n", thread_data->thread_num);
	else
		printf("I exited because someone died: %d\n", thread_data->thread_num);
	thread_data->common_data->philo_died = true;
	pthread_mutex_unlock(thread_data->common_data->data_mutex);
	return 0;
}

int main() {


	int num_threads = 100;
	unsigned long long time_to_die = 50 * 1000;

	pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
	if (!threads)
		exit_perror("malloc");
	t_thread_data *thread_data = malloc(sizeof(t_thread_data) * num_threads);
	if (!thread_data) {
		free(threads);
		exit_perror("malloc");
	}
	pthread_mutex_t fork;
	pthread_mutex_init(&fork, NULL);

	pthread_mutex_t data_mutex;
	pthread_mutex_init(&data_mutex, NULL);


	struct timeval tv;
	if (gettimeofday(&tv, NULL))
		return -1;
	unsigned long long base_time = (unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000;
	t_common_data data = {false, true, base_time, time_to_die, &data_mutex, &fork};
	for (int i = 0; i < num_threads; i++) {
		thread_data[i].thread_num = i;
		thread_data[i].time_since_last_meal = 0;
		thread_data[i].common_data = &data;
	}


	for (int i = 0; i < num_threads; ++i)
		pthread_create(&threads[i], NULL, philo_loop, &thread_data[i]);

	for (int i = 0; i < num_threads; ++i)
		pthread_join(threads[i], NULL);

	/* calculate passed time since launch */

	if (gettimeofday(&tv, NULL))
		return -1;
	unsigned long long end = ((unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000) - base_time;
	printf("timestamp in c: %lld\n", end);

	free(threads);
	free(thread_data);
	exit(EXIT_SUCCESS);
}
