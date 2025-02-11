#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void *fun(void *param) {
	(void)param;
	usleep(100);
	printf("Pring a sentence to stdout");
	printf("\n");
	return NULL;
}

int main(int argc, char *argv[]) {

	int num_threads = 2000;
	pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
	for (int i = 0; i < num_threads; ++i)
		pthread_create(&threads[i], NULL, fun, NULL);
	for (int i = 0; i < num_threads; ++i)
		pthread_join(threads[i], NULL);
	return 0;
}
