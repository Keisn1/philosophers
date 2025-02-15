/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfreyer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 10:00/37 by kfreyer           #+#    #+#             */
/*   Updated: 2025/02/15 10:00:37 by kfreyer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

char	*validate(char **argv)
{
	int		count;
	char	*str;

	count = 0;
	while (argv[count])
	{
		str = argv[count++];
		while (*str)
		{
			if (*str < '0' || *str > '9')
				return ("non-valid argument");
			str++;
		}
		if (count > 5)
			return ("too many arguments");
	}
	if (count < 4)
		return ("missing values");
	return (NULL);
}

int	parse(char *str, unsigned long long *nbr)
{
	unsigned long long	left_over;

	*nbr = 0;
	while (*str)
	{
		left_over = ULLONG_MAX / 10;
		if (left_over < *nbr)
		{
			*nbr = 0;
			return (-1);
		}
		*nbr *= 10;
		if (ULLONG_MAX - *nbr < (unsigned long long)(*str - '0'))
		{
			*nbr = 0;
			return (-1);
		}
		*nbr += (*str - '0');
		str++;
	}
	return (0);
}
