#include "philo.h"

void exit_perror(char *err_msg) {
	perror(err_msg);
	exit(EXIT_FAILURE);
}

/* the 1 milisecond that uneven philos wait is to make it more synchronized and faster */
/* we need all of the even philosopher grab a fork to their right so that the uneven block on grabing the fork to the left*/
/* otherwise an even philosopher might wait for too long, because there would be a combination of a fork where a philosopher could eat */
/* but this combination is actually not realised */
void *philo_routine(void *params) {
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
		printf("%lld %d has taken a fork\n", get_timestamp() - philo->params.base_time, philo->philo_num);
		printf("%lld %d has taken a fork\n", get_timestamp() - philo->params.base_time, philo->philo_num);
		pthread_mutex_unlock(philo->shared->stdout_lock);

		bool philo_dead = false;
		unsigned long long start = get_timestamp();
		unsigned long long time_to_eat = philo->params.time_to_eat;
		philo->time_last_meal = start;
		while ((get_timestamp() - start) < time_to_eat) {
			pthread_mutex_lock(philo->shared->check_lock);
			philo_dead = philo->shared->philo_died;
			pthread_mutex_unlock(philo->shared->check_lock);
			if (philo_dead)
				break;
			usleep(100);
		}
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
		if (philo_dead)
			break;


		pthread_mutex_lock(philo->shared->stdout_lock);
		printf("%lld %d is sleeping\n", get_timestamp() - philo->params.base_time, philo->philo_num);
		pthread_mutex_unlock(philo->shared->stdout_lock);
		start = get_timestamp();
		unsigned long long time_to_sleep = 100;
		while ((get_timestamp() - start) < time_to_sleep) {
			pthread_mutex_lock(philo->shared->check_lock);
			philo_dead = philo->shared->philo_died;
			pthread_mutex_unlock(philo->shared->check_lock);
			if (philo_dead)
				break;
			usleep(100);
		}
		if (philo_dead)
			break;
	}
	return NULL;
}

void *observer_routine(void *params) {
	t_observer *observer = (t_observer*)params;
	bool philo_dead = false;
	while (!philo_dead) {
		int i = 0;
		pthread_mutex_lock(observer->shared->check_lock);
		while (i < observer->num_philos) {
			if ((get_timestamp() - observer->philos[i].time_last_meal) > observer->params.time_to_die) {
				observer->shared->philo_died = true;
				philo_dead = true;
			}
			i++;
		}
		pthread_mutex_unlock(observer->shared->check_lock);
		usleep(100);
	}
	return NULL;
}

int main() {
	int num_philos = 2;

	unsigned long long base_time = get_timestamp();
	unsigned long long time_to_die = 50;
	unsigned long long time_to_eat = 100;

	pthread_t *philo_threads = malloc(sizeof(pthread_t) * num_philos);
	if (!philo_threads)
		exit_perror("malloc");

	t_philo *philos = init_philos(num_philos);
	set_philo_params(philos, num_philos, (t_params){base_time, time_to_die, time_to_eat});

	for (int i = 0; i < num_philos; ++i)
		pthread_create(&philo_threads[i], NULL, philo_routine, &philos[i]);

	for (int i = 0; i < num_philos; ++i)
		pthread_join(philo_threads[i], NULL);

	teardown(philos, num_philos, philo_threads);
	return 0;
}
