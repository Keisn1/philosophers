#include "philo.h"
#include <pthread.h>
#include <unistd.h>

void exit_perror(char *err_msg) {
	perror(err_msg);
	exit(EXIT_FAILURE);
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

	teardown_main(philos, num_philos, philo_threads);
	return 0;
}
