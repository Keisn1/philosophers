#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void exit_perror(char *error) {
	perror("malloc");
	exit(EXIT_FAILURE);
}

typedef struct s_thread_data {
	int thread_num;
	pthread_mutex_t fork;
} t_thread_data;

void *func3(void *param) {

	t_thread_data *thread_data = (t_thread_data*)param;

	printf("Thread %d got fork\n", thread_data->thread_num);
	usleep(100);
	printf("Thread %d put down fork\n", thread_data->thread_num);
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
		data[i].fork =  forks[i];
	}

	for (int i = 0; i < num_threads; ++i)
		pthread_create(&threads[i], NULL, func3, &data[i]);

	for (int i = 0; i < num_threads; ++i)
		pthread_join(threads[i], NULL);

	free(threads);
	free(forks);
	exit(EXIT_SUCCESS);
}
