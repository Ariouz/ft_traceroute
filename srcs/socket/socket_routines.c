#include "ft_ping.h"

void *sender_routine(void *arg)
{
	(void) arg;
	int sequence = 0;
	while (true)
	{
		send_socket(g_opts->target_ip, g_opts->sockfd, 56, sequence);
		sequence++;
		sleep(1);

		pthread_mutex_lock(&g_opts_mutex);
		if (!g_opts->is_running)
		{
			pthread_mutex_unlock(&g_opts_mutex);
			break ;
		}
		pthread_mutex_unlock(&g_opts_mutex);
	}
	return NULL;
}

void *receiver_routine(void *arg)
{
	(void) arg;
	fd_set read_set;
	int		sockfd;

	pthread_mutex_lock(&g_opts_mutex);
	sockfd = g_opts->sockfd;
	pthread_mutex_unlock(&g_opts_mutex);

	while (true)
	{
		FD_ZERO(&read_set);
		FD_SET(sockfd, &read_set);
		
		receive_socket(sockfd, &read_set);
		
		pthread_mutex_lock(&g_opts_mutex);
		if (!g_opts->is_running)
		{
			pthread_mutex_unlock(&g_opts_mutex);
			break ;
		}
		pthread_mutex_unlock(&g_opts_mutex);
	}
	FD_CLR(g_opts->sockfd, &read_set);
	return NULL;
}