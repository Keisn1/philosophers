#include "philo.h"
#include <pthread.h>

void exit_perror(char *err_msg) {
	perror(err_msg);
	exit(EXIT_FAILURE);
}

bool check_philo_died(t_philo *philo) {
	pthread_mutex_lock(philo->shared->check_lock);
	if (philo->shared->philo_died) {
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->shared->check_lock);
		return true;
	}
	pthread_mutex_unlock(philo->shared->check_lock);
	return false;
}

void print_fork_msg(t_philo *philo) {
	pthread_mutex_lock(philo->shared->stdout_lock);
	printf("%lld %d has taken a fork\n", get_timestamp() - philo->params.base_time, philo->philo_num);
	printf("%lld %d has taken a fork\n", get_timestamp() - philo->params.base_time, philo->philo_num);
	pthread_mutex_unlock(philo->shared->stdout_lock);
}

void print_sleep_msg(t_philo *philo) {
	pthread_mutex_lock(philo->shared->stdout_lock);
	printf("%lld %d is sleeping\n", get_timestamp() - philo->params.base_time, philo->philo_num);
	pthread_mutex_unlock(philo->shared->stdout_lock);
}

void print_thinking_msg(t_philo *philo) {
	pthread_mutex_lock(philo->shared->stdout_lock);
	printf("%lld %d is thinking\n", get_timestamp() - philo->params.base_time, philo->philo_num);
	pthread_mutex_unlock(philo->shared->stdout_lock);
}

void set_last_meal(t_philo *philo, unsigned long long start) {
	pthread_mutex_lock(philo->shared->check_lock);
	philo->time_last_meal = start;
	pthread_mutex_unlock(philo->shared->check_lock);
}

bool sleep_loop(t_philo *philo, unsigned long long start, unsigned long long time) {
	bool philo_dead = false;
	while ((get_timestamp() - start) < time) {
		pthread_mutex_lock(philo->shared->check_lock);
		philo_dead = philo->shared->philo_died;
		pthread_mutex_unlock(philo->shared->check_lock);
		if (philo_dead)
			return true;
		usleep(100);
	}
	return false;
}

bool eat(t_philo *philo) {
	if (philo->philo_num % 2 == 0) {
		pthread_mutex_lock(philo->r_fork);
		pthread_mutex_lock(philo->l_fork);
	} else {
		pthread_mutex_lock(philo->l_fork);
		pthread_mutex_lock(philo->r_fork);
	}
	if (check_philo_died(philo))
		return true;
	print_fork_msg(philo);

	bool philo_dead = false;
	unsigned long long start = get_timestamp();
	unsigned long long time_to_eat = philo->params.time_to_eat;

	set_last_meal(philo, start);
	philo_dead = sleep_loop(philo, start, time_to_eat);

	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
	return philo_dead;
}

bool sleeping(t_philo *philo) {
	print_sleep_msg(philo);
	unsigned long long start = get_timestamp();
	unsigned long long time_to_sleep = 100;
	while ((get_timestamp() - start) < time_to_sleep) {
		pthread_mutex_lock(philo->shared->check_lock);
		if (philo->shared->philo_died) {
			pthread_mutex_unlock(philo->shared->check_lock);
			return true;
		}
		pthread_mutex_unlock(philo->shared->check_lock);
		usleep(100);
	}
	return false;
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
		if (eat(philo))
			break;
		if (sleeping(philo))
			break;
		print_thinking_msg(philo);
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
				pthread_mutex_lock(observer->shared->stdout_lock);
				printf("%lld %d died\n", get_timestamp() - observer->params.base_time, i+1);
				pthread_mutex_unlock(observer->shared->stdout_lock);
				break;
			}
			i++;
		}
		pthread_mutex_unlock(observer->shared->check_lock);
		if (philo_dead)
			break;
		usleep(100);
	}
	return NULL;
}

int main() {
	int num_philos = 200;

	unsigned long long base_time = get_timestamp();
	unsigned long long time_to_die = 50;
	unsigned long long time_to_eat = 100;

	pthread_t *philo_threads = malloc(sizeof(pthread_t) * num_philos);
	if (!philo_threads)
		exit_perror("malloc");
	pthread_t observer_thread;

	t_philo *philos = init_philos(num_philos);
	set_philo_params(philos, num_philos, (t_params){base_time, time_to_die, time_to_eat});
	t_observer observer;
	observer.num_philos = num_philos;
	observer.philos = philos;
	observer.shared = philos[0].shared;
	observer.params = philos[0].params;

	pthread_create(&observer_thread, NULL, observer_routine, &observer);
	for (int i = 0; i < num_philos; ++i)
		pthread_create(&philo_threads[i], NULL, philo_routine, &philos[i]);

	pthread_join(observer_thread, NULL);
	for (int i = 0; i < num_philos; ++i)
		pthread_join(philo_threads[i], NULL);

	teardown(philos, num_philos, philo_threads);
	return 0;
}
