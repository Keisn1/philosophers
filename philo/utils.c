#include <unistd.h>
#include <sys/time.h>


unsigned long long get_timestamp(unsigned long long base_time) {
	struct timeval tv;
	if (gettimeofday(&tv, NULL))
		return -1;
	return ((unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000) - base_time;
}
