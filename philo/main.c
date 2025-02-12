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

/* the 1 milisecond that uneven philos wait is to make it more synchronized and faster */
/* we need all of the even philosopher grab a fork to their right so that the uneven block on grabing the fork to the left*/
/* otherwise an even philosopher might wait for too long, because there would be a combination of a fork where a philosopher could eat */
/* but this combination is actually not realised */
void *start_routine(void *params) {
	t_philo* philo = (t_philo*)params;
	if (philo->philo_num % 2)
		usleep(1000);
	while (1) {
		if (philo->philo_num % 2 == 0) {
			pthread_mutex_lock(philo->r_fork);
			pthread_mutex_lock(philo->l_fork);
		} else {
			pthread_mutex_lock(philo->l_fork);
			pthread_mutex_lock(philo->r_fork);
		}
		pthread_mutex_lock(philo->shared->stdout_lock);
		printf("%lld %d has taken a fork\n", get_timestamp(philo->params.base_time), philo->philo_num);
		printf("%lld %d has taken a fork\n", get_timestamp(philo->params.base_time), philo->philo_num);
		pthread_mutex_unlock(philo->shared->stdout_lock);

		usleep(philo->params.time_to_eat * 1000);
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);

		pthread_mutex_lock(philo->shared->stdout_lock);
		printf("%lld %d is sleeping\n", get_timestamp(philo->params.base_time), philo->philo_num);
		pthread_mutex_unlock(philo->shared->stdout_lock);
		break;
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
	}
	return NULL;
}

int main() {
	int num_philos = 100;

	unsigned long long base_time = get_timestamp(0);
	unsigned long long time_to_die = 50;
	unsigned long long time_to_eat = 100;

	pthread_t *threads = malloc(sizeof(pthread_t) * num_philos);
	if (!threads)
		exit_perror("malloc");

	t_philo *philos = init_philos(num_philos);
	set_philo_params(philos, num_philos, (t_params){base_time, time_to_die, time_to_eat});


	for (int i = 0; i < num_philos; ++i)
		pthread_create(&threads[i], NULL, start_routine, &philos[i]);

	for (int i = 0; i < num_philos; ++i)
		pthread_join(threads[i], NULL);

	teardown(philos, num_philos, threads);
	return 0;
}
