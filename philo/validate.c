#include "philo.h"

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

unsigned long long parse(char *arg) {
	(void)arg;
	return 10;
}
