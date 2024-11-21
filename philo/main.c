#include "philo.h"
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

struct thread_info {    /* Used as argument to thread_start() */
	unsigned long long timestamp;        /* ID returned by pthread_create() */
};


static void* routine(void* arg) {
	struct thread_info *tinfo = (struct thread_info*)arg;
	thinking_msg(1);
	dying_msg(1, tinfo->timestamp);
	return NULL;
}


int main(int argc, char** argv) {
	(void)argc;
	pthread_t thread;
	struct thread_info  *tinfo;

    tinfo = malloc( sizeof(*tinfo));
    if (tinfo == NULL)
		exit(EXIT_FAILURE);
	tinfo->timestamp = (unsigned long long)ft_atoi(argv[2]);

	if(pthread_create(&thread, NULL, routine, tinfo))
		exit(EXIT_FAILURE);

	if (pthread_join(thread, NULL))
		exit(EXIT_FAILURE);

	return 0;
}
