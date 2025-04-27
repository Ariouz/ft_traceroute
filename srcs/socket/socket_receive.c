#include "ft_ping.h"

size_t wait_socket(int sockfd, fd_set *read_set, struct timeval timeout)
{
    int res = 0;
    res = select(sockfd + 1, read_set, NULL, NULL, &timeout);
	if (!is_runnning()) return -1;
	if (res == -1)
		fatal_error(strerror(errno));

	if (res == 0)
	{
		printf("Request timed out\n");
		return -1;
	}
    return (size_t) res;
}

void receive_socket(int sockfd, fd_set *read_set)
{
    struct timeval 	timeout = {3, 0};
	size_t 			rcv_bytes = 0;
	char			rcv_buf[1024];

	rcv_bytes = wait_socket(sockfd, read_set, timeout);
    if (rcv_bytes == (size_t) -1) return ;

	if (!FD_ISSET(sockfd, read_set)) return ;

    struct sockaddr_in rcv_sockaddr;
	socklen_t slen;

	slen = sizeof(struct sockaddr_in);
	rcv_bytes = recvfrom(sockfd, rcv_buf, sizeof(rcv_buf), 0, (struct sockaddr *) &rcv_sockaddr, &slen);

	if (rcv_bytes < (size_t) (sizeof(struct iphdr) + sizeof(struct icmphdr))) {
		return ;
	}
	
	struct iphdr *rcv_ip = (struct iphdr *)rcv_buf;
    struct icmphdr *rcv_icmp = (struct icmphdr *)(rcv_buf + sizeof(struct iphdr));

	int ip_header_len = rcv_ip->ihl * 4;
    int rvc_data_len = rcv_bytes - ip_header_len;
    int ttl = rcv_ip->ttl;
	int sequence = rcv_icmp->un.echo.sequence;

	pthread_mutex_lock(&g_stats_mutex);
	float rtt = save_rcv_time(g_stats, sequence);
	pthread_mutex_unlock(&g_stats_mutex);

	if (rcv_icmp->type == ICMP_ECHOREPLY) {
		printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n", 
			rvc_data_len, to_str(rcv_sockaddr.sin_addr), sequence, ttl, rtt);
		
		pthread_mutex_lock(&g_stats_mutex);
		process_rtt(rtt);
		g_stats->rcvd_sockets++;
		pthread_mutex_unlock(&g_stats_mutex);
	} else {
		pthread_mutex_lock(&g_stats_mutex);
		if (get_option(OPT_VERBOSE)->value == true)
		{
			printf("From %s: icmp_seq=%d type=%d code=%d\n", 
				to_str(rcv_sockaddr.sin_addr), sequence, rcv_icmp->type, rcv_icmp->code);
		}
		pthread_mutex_unlock(&g_stats_mutex);
	}

}