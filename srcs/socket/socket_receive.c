#include "ft_traceroute.h"

size_t wait_socket(int sockfd, fd_set *read_set)
{
	struct timeval timeout = {1, 0};
    int res = 0;
    res = select(sockfd + 1, read_set, NULL, NULL, &timeout);
	if (!is_runnning()) return -1;
	if (res == -1)
		fatal_error(strerror(errno));
	if (res == 0) return -1;

    return (size_t) res;
}

void receive_socket(int sockfd, fd_set *read_set)
{
	size_t 			rcv_bytes = 0;
	char			rcv_buf[1024];

	check_timeouts();
	rcv_bytes = wait_socket(sockfd, read_set);
    if (rcv_bytes == (size_t) -1) return ;

	if (!FD_ISSET(sockfd, read_set)) return ;

    struct sockaddr_in rcv_sockaddr;
	socklen_t slen;

	slen = sizeof(struct sockaddr_in);
	rcv_bytes = recvfrom(sockfd, rcv_buf, sizeof(rcv_buf), 0, (struct sockaddr *) &rcv_sockaddr, &slen);

	if (rcv_bytes < (size_t) (sizeof(struct iphdr) + sizeof(struct icmphdr))) return ;
	
	struct iphdr *rcv_ip = (struct iphdr *)rcv_buf;
    struct icmphdr *rcv_icmp = (struct icmphdr *)(rcv_buf + sizeof(struct iphdr));

	int ip_header_len = rcv_ip->ihl * 4;
    int rvc_data_len = rcv_bytes - ip_header_len;
    int ttl = rcv_ip->ttl;
	int sequence = rcv_icmp->un.echo.sequence;

	float rtt = save_rcv_time(g_stats, sequence);

	if (rcv_icmp->type == ICMP_ECHOREPLY) {
		if (get_option(OPT_QUIET)->value == false)
			printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.3f ms\n", 
				rvc_data_len, to_str(rcv_sockaddr.sin_addr), to_str(rcv_sockaddr.sin_addr), sequence, ttl, rtt);
	} else {
		handle_icmp_types(rcv_sockaddr, rcv_icmp, rcv_bytes, rcv_ip);
	}

}

void	handle_icmp_types(struct sockaddr_in rcv_sockaddr, struct icmphdr *rcv_icmp, size_t rcv_bytes, struct iphdr *rcv_ip)
{
	int rvc_data_len = rcv_bytes - rcv_ip->ihl * 4;
	int sequence = rcv_icmp->un.echo.sequence;
	
	int type = rcv_icmp->type;
	int code = rcv_icmp->code;

	if (get_option(OPT_QUIET)->value == true) return ;

	// err from sender
	if (rcv_icmp->un.echo.id == 0)
	{
		if (type == ICMP_TIME_EXCEEDED && code == ICMP_EXC_TTL)
			printf("%d bytes from %s: Time to live exceeded\n", rvc_data_len, to_str(rcv_sockaddr.sin_addr));
	}
	else if (get_option(OPT_VERBOSE)->value == true) // err from dest/router
	{
		printf("From %s: icmp_seq=%d type=%d code=%d id=%d\n", 
			to_str(rcv_sockaddr.sin_addr), sequence , rcv_icmp->type, rcv_icmp->code, rcv_icmp->un.echo.id);
	}

}