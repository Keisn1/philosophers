#include "philo.h"

void	*teardown_1(pthread_mutex_t **forks, int i, pthread_mutex_t *stdout_lock, t_shared_data *shared)
{
	printf("Allocation failed\n");
	while (i-- > 0)
	{
		pthread_mutex_destroy(forks[i]);
		free(forks[i]);
	}
	free(forks);
	pthread_mutex_destroy(stdout_lock);
	free(stdout_lock);
	free(shared);
	return (NULL);
}

void	*teardown_2(pthread_mutex_t **forks, int i,
		pthread_mutex_t *stdout_lock, t_shared_data *shared)
{
	printf("Mutex initialization failed\n");
	free(forks[i]);
	while (i-- > 0)
	{
		pthread_mutex_destroy(forks[i]);
		free(forks[i]);
	}
	free(forks);
	pthread_mutex_destroy(stdout_lock);
	free(stdout_lock);
	free(shared);
	return (NULL);
}

void	*teardown_3(pthread_mutex_t *stdout_lock, t_shared_data *shared)
{
	printf("Allocation failed\n");
	pthread_mutex_destroy(stdout_lock);
	free(stdout_lock);
	free(shared);
	return (NULL);
}

void	*teardown_4(pthread_mutex_t *stdout_lock, t_shared_data *shared)
{
	printf("Mutex initialization failed\n");
	free(stdout_lock);
	free(shared);
	return (NULL);
}

void	*teardown_5(t_shared_data *shared)
{
	printf("Allocation failed\n");
	free(shared);
	return (NULL);
}

void	*teardown_6(void)
{
	printf("Allocation failed\n");
	return (NULL);
}
