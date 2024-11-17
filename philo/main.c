#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

int main() {
	int count = 0;

	struct timeval tv;

	if (gettimeofday(&tv, NULL))
		return -1;
	long long base = (long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000;
	while (1) {
		usleep(1000);

		if (gettimeofday(&tv, NULL))
			return -1;
		long long end = ((long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000) - base;

		printf("timestamp in c: %lld\n", end);
		fflush(stdout);
		count++;
		if (count == 10000000) {
			break;
		}
	}
    return 0;
}
