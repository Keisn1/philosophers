#include "philo.h"
#include <bits/pthreadtypes.h>
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>


void thinking_msg(int philo) {
	ft_putnbr_ull_fd(philo, STDOUT_FILENO);
	ft_putstr_fd(" is thinking\n", STDOUT_FILENO);
}

void dying_msg(int philo, unsigned long long timestamp) {
	ft_putnbr_ull_fd(timestamp, STDOUT_FILENO);
	ft_putstr_fd(" ", STDOUT_FILENO);
	ft_putnbr_ull_fd(philo, STDOUT_FILENO);
	ft_putstr_fd(" died\n", STDOUT_FILENO);
}


static void* routine(void* arg) {
	struct s_thread_info *tinfo = (struct s_thread_info*)arg;
	thinking_msg(1);
	dying_msg(1, tinfo->timestamp);
	return NULL;
}

int main(int argc, char** argv) {
	(void)argc;
	pthread_t thread;
	struct s_thread_info  *thread_info;

	pthread_mutex_t mutex;
	const pthread_mutexattr_t *mu_attr = "fast";

	pthread_mutex_init(&mutex, mu_attr);
	thread_info = malloc( sizeof(*thread_info));
	if (thread_info == NULL)
		exit(EXIT_FAILURE);

	thread_info->timestamp = (unsigned long long)ft_atoi(argv[2]);

	if(pthread_create(&thread, NULL, routine, thread_info))
		exit(EXIT_FAILURE);

	if (pthread_join(thread, NULL))
		exit(EXIT_FAILURE);

	free(thread_info);

	return 0;
}
