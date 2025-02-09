#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void exit_perror(char *err_msg) {
	perror(err_msg);
	exit(EXIT_FAILURE);
}

typedef struct s_thread_data {
	int thread_num;
	pthread_mutex_t *fork_left;
	pthread_mutex_t *fork_right;
} t_thread_data;

void *func3(void *param) {

	t_thread_data *thread_data = (t_thread_data*)param;

	pthread_mutex_lock(thread_data->fork_left);
	printf("Thread %d got fork\n", thread_data->thread_num);
	usleep(100);
	printf("Thread %d put down fork\n", thread_data->thread_num);
	pthread_mutex_unlock(thread_data->fork_left);
	return 0;
}

int main() {
	int num_threads = 10;

	pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
	if (!threads)
		exit_perror("malloc");
	t_thread_data *data = malloc(sizeof(t_thread_data) * num_threads);
	if (!data) {
		free(threads);
		exit_perror("malloc");
	}
	pthread_mutex_t *forks = malloc(sizeof(pthread_mutex_t) * num_threads);
	if (!forks) {
		free(data);
		free(threads);
		exit_perror("malloc");
	}

	for (int i = 0; i < num_threads; i++) {
		pthread_mutex_init(&forks[i], NULL);
		data[i].thread_num = i;
		data[i].fork_left =  &forks[i];
		if (i == num_threads-1)
			data[i].fork_left =  &forks[0];
		else
			data[i].fork_right =  &forks[i+1];
	}

	for (int i = 0; i < num_threads; ++i)
		pthread_create(&threads[i], NULL, func3, &data[i]);

	for (int i = 0; i < num_threads; ++i)
		pthread_join(threads[i], NULL);

	free(threads);
	free(data);
	free(forks);
	exit(EXIT_SUCCESS);
}
