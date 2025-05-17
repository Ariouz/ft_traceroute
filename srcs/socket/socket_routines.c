#include "ft_traceroute.h"

void *sender_routine(void *arg)
{
	(void) arg;
	int sequence = 0;
	while (true)
	{
		send_socket(g_opts->target_ip, g_opts->sockfd, get_option(OPT_PACKET_SIZE)->value, sequence);
		sequence++;
		
		if (multisleep(get_option(OPT_INTERVAL)->value * 1000, 200) == -1) break ;
	}
	return NULL;
}

void *receiver_routine(void *arg)
{
	(void) arg;
	fd_set read_set;
	int		sockfd;

	sockfd = g_opts->sockfd;

	while (true)
	{
		FD_ZERO(&read_set);
		FD_SET(sockfd, &read_set);

		receive_socket(sockfd, &read_set);
		
		if (!g_opts->is_running)
		{
			break ;
		}
	}
	FD_CLR(g_opts->sockfd, &read_set);
	return NULL;
}