/* test with 2 philosophers where one of them shall die in about 100 ms only one fork */
#include <pthread.h>
#include <sys/time.h>
#include "philo.h"

void exit_perror(char *err_msg) {
	perror(err_msg);
	exit(EXIT_FAILURE);
}


/* void *philo_loop(void *param) { */
/* 	t_philo *thread_data = (t_philo*)param; */

/* 	while (1) { */
/* 		pthread_mutex_lock(thread_data->protected->data_mutex); */
/* 		if (thread_data->protected->fork_available) */
/* 		{ */
/* 			/\* lock the fork and unlock data_mutex *\/ */
/* 			printf("Thread %d got fork\n", thread_data->thread_num); */
/* 			thread_data->protected->fork_available = false; */
/* 			pthread_mutex_unlock(thread_data->protected->data_mutex); */

/* 			int count2 = 0; */
/* 			bool philo_dead = false; */
/* 			while (count2 < 10000 && !philo_dead) { */
/* 				usleep(1 * 1000); */
/* 				count2++; */
/* 				pthread_mutex_lock(thread_data->protected->data_mutex); */
/* 				philo_dead = thread_data->protected->philo_died; */
/* 				pthread_mutex_unlock(thread_data->protected->data_mutex); */
/* 			} */
/* 			if (philo_dead) */
/* 				break; */

/* 			pthread_mutex_lock(thread_data->protected->data_mutex); */
/* 			printf("Thread %d put down fork\n", thread_data->thread_num); */
/* 			thread_data->protected->fork_available = true; */
/* 			pthread_mutex_unlock(thread_data->protected->data_mutex); */
/* 		} else { */
/* 			if (thread_data->protected->philo_died == true) { */
/* 				pthread_mutex_unlock(thread_data->protected->data_mutex); */
/* 				break; */
/* 			} */

/* 			unsigned long long time = get_timestamp(thread_data->params.base_time); */
/* 			if (time - thread_data->time_last_meal > thread_data->params.time_to_die) { */
/* 				thread_data->protected->philo_died = true; */
/* 				printf("I'm dead: %d : %lld\n", thread_data->thread_num, time); */
/* 				pthread_mutex_unlock(thread_data->protected->data_mutex); */
/* 				break; */
/* 			} */

/* 			pthread_mutex_unlock(thread_data->protected->data_mutex); */
/* 			usleep(1 * 1000); */
/* 		} */
/* 	} */
/* 	return 0; */
/* } */

void *start_routine(void *params) {
	(void)params;
	return NULL;
}

int main() {
	int num_philos = 200;

	unsigned long long base_time = get_timestamp(0);
	unsigned long long time_to_die = 50;

	pthread_t *threads = malloc(sizeof(pthread_t) * num_philos);
	if (!threads)
		exit_perror("malloc");

	t_philo *philos = init_philos(num_philos, base_time, time_to_die);
	for (int i = 0; i < num_philos; ++i)
		pthread_create(&threads[i], NULL, start_routine, &philos[i]);

	for (int i = 0; i < num_philos; ++i)
		pthread_join(threads[i], NULL);

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
	return 0;
}
