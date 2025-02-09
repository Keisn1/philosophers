#include "philo.h"
#include <cstdlib>
#include <limits.h>
#include <stdio.h>

char *validate(char **argv) {
	int count = 0;
	while (argv[count]) {
		char* str = argv[count++];
		while (*str) {
			if (*str < '0' || *str > '9')
				return "non-valid argument";
			str++;
		}
		if (count > 5)
			return "too many arguments";
	}
	if (count < 4)
		return "missing values";
	return NULL;
}

int parse(char* str, unsigned int *nbr)  {
	*nbr = 0;
	while (*str) {
		unsigned int left_over = UINT_MAX / 10;
		if (left_over < *nbr ) {
			*nbr = 0;
			return -1;
		}
		*nbr *= 10;
		if (UINT_MAX - *nbr  <  (unsigned int)(*str - '0')) {
			*nbr = 0;
			return -1;
		}
		*nbr += (*str - '0');
		str++;
	}
	return 0;
}
