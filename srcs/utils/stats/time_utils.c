#include "ft_ping.h"

long long get_time_ms(struct timeval tv) {
    return (long long)(tv.tv_sec) * 1000 + (long long)(tv.tv_usec) / 1000;
}

long long get_time_us(struct timeval tv) {
    return (long long)(tv.tv_sec) * (1000*1000) + (long long)(tv.tv_usec);
}

int		multisleep(long max_time_millis, long increment)
{
	long total_time_millis;

	total_time_millis = 0;
	while (total_time_millis < max_time_millis)
	{
		total_time_millis += increment;
		usleep(increment * 1000);
		pthread_mutex_lock(&g_opts_mutex);
		if (!g_opts->is_running)
		{
			pthread_mutex_unlock(&g_opts_mutex);
			return -1;
		}
		pthread_mutex_unlock(&g_opts_mutex);
	}
	return 0;
}