#include "philo.h"
#include <pthread.h>
#include <unistd.h>

void exit_perror(char *err_msg) {
	perror(err_msg);
	exit(EXIT_FAILURE);
}

bool check_philo_died(t_philo *philo) {
	pthread_mutex_lock(philo->shared->check_lock);
	if (philo->shared->philo_died) {
		give_up_forks(philo);
		pthread_mutex_unlock(philo->shared->check_lock);
		return true;
	}
	pthread_mutex_unlock(philo->shared->check_lock);
	return false;
}

void set_last_meal(t_philo *philo, unsigned long long start) {
	pthread_mutex_lock(philo->shared->check_lock);
	if (!philo->shared->philo_died) {
		philo->time_last_meal = start;
		pthread_mutex_lock(philo->shared->stdout_lock);
		printf("%lld %d is eating\n", philo->time_last_meal - philo->params.base_time, philo->philo_num);
		pthread_mutex_unlock(philo->shared->stdout_lock);
	}
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

bool get_forks(t_philo *philo) {
	bool has_r_fork = false;
	bool has_l_fork = false;
	while (!has_r_fork || !has_l_fork) {
		if (philo->philo_num % 2 == 0) {
			if (!has_r_fork) {
				has_r_fork = try_to_get_r_fork(philo);
				if (has_r_fork)
					print_fork_msg(philo);
			}
			if (!has_l_fork && has_r_fork) {
				has_l_fork = try_to_get_l_fork(philo);
				if (has_l_fork) {
					print_fork_msg(philo);
					set_last_meal(philo, get_timestamp());
				}
			}
		} else {
			if (!has_l_fork) {
				has_l_fork = try_to_get_l_fork(philo);
				if (has_l_fork)
					print_fork_msg(philo);
			}
			if (!has_r_fork && has_l_fork) {
				has_r_fork = try_to_get_r_fork(philo);
				if (has_r_fork) {
					print_fork_msg(philo);
					set_last_meal(philo, get_timestamp());
				}
			}
		}
		if (check_philo_died(philo)) 
			return true;
		usleep(100);
	}
	return false;
}

bool eat(t_philo *philo) {
	bool philo_dead = false;
	if (get_forks(philo))
		return true;;
	philo_dead = sleep_loop(philo, get_timestamp(), philo->params.time_to_eat);
	if (philo_dead) {
		give_up_forks(philo);
		return philo_dead;
	}
	philo->meals_eaten++;
	pthread_mutex_lock(philo->shared->check_lock);
	if (philo->meals_eaten == philo->params.must_eat)
		philo->ate_enough = 1;
	pthread_mutex_unlock(philo->shared->check_lock);
	give_up_forks(philo);
	return false;
}

bool sleeping(t_philo *philo) {
	print_sleep_msg(philo);
	unsigned long long start = get_timestamp();
	unsigned long long time_to_sleep = philo->params.time_to_sleep;
	return sleep_loop(philo, start, time_to_sleep);
}

void *philo_routine(void *params) {
	t_philo* philo = (t_philo*)params;
	if (philo->philo_num % 2 == 0)
		usleep(1000);
	while (1) {
		if (eat(philo))
			break;
		if (sleeping(philo))
			break;
		print_thinking_msg(philo);
		if ((philo->params.time_to_die - (get_timestamp() - philo->time_last_meal)) > 10)
			usleep(1000);
	}
	return NULL;
}

int main(int argc, char** argv) {
	check_args(argc, argv);

	unsigned long long num_philos = get_num_philos(argv[1]);
	unsigned long long time_to_die = get_ull(argv[2]);
	unsigned long long time_to_eat = get_ull(argv[3]);
	unsigned long long time_to_sleep = get_ull(argv[4]);
	unsigned long long must_eat;
	if (argc == 6) {

		must_eat = get_ull(argv[5]);
		if (must_eat == 0) {
			printf("Philosophers must eat!\n");
			exit(EXIT_FAILURE);
		}
	}
	else
		must_eat = 0;
	if (time_to_die < 1) {
		exit(EXIT_FAILURE);
	}
	if (time_to_eat < 1) {
		exit(EXIT_FAILURE);
	}
	if (time_to_sleep < 1) {
		exit(EXIT_FAILURE);
	}

	pthread_t *philo_threads = malloc(sizeof(pthread_t) * num_philos);
	if (!philo_threads)
		exit_perror("malloc");
	pthread_t observer_thread;

	t_philo *philos = init_philos(num_philos);
	unsigned long long base_time = get_timestamp();
	set_philo_params(philos, num_philos, (t_params){base_time, time_to_die, time_to_eat, time_to_sleep, must_eat});
	t_observer observer;
	observer.num_philos = num_philos;
	observer.philos = philos;
	observer.shared = philos[0].shared;
	observer.params = philos[0].params;

	pthread_create(&observer_thread, NULL, observer_routine, &observer);
	for (unsigned long long i = 0; i < num_philos; ++i)
		pthread_create(&philo_threads[i], NULL, philo_routine, &philos[i]);

	pthread_join(observer_thread, NULL);
	for (unsigned long long i = 0; i < num_philos; ++i)
		pthread_join(philo_threads[i], NULL);

	teardown(philos, num_philos, philo_threads);
	return 0;
}
