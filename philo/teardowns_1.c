/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_teardowns_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 09:37/58 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/15 09:37:58 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
