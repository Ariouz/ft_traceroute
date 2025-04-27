#include "ft_ping.h"

int     open_socket()
{
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd == -1)
	{
		fatal_error(strerror(errno));
	}
    return sockfd;
}

struct sockaddr_in  get_sockaddr(struct in_addr target_ip)
{
    struct sockaddr_in sockaddr;
	sockaddr.sin_addr = target_ip;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = 0;

    return sockaddr;
}

struct icmphdr  build_icmp_header(unsigned char *data, int sequence, size_t payload_size)
{
    struct icmphdr hdr;
	memset(&hdr, 0, sizeof(hdr));
	hdr.type = ICMP_ECHO;
	hdr.code = 0;
	
	hdr.un.echo.id = getpid() & 0xFFFF;
	hdr.un.echo.sequence = sequence;

	memcpy(data, &hdr, sizeof(hdr));

	char payload[payload_size];
	memset(&payload, '0', payload_size);
	memcpy(data + sizeof(hdr), payload, payload_size);

	hdr.checksum = checksum(data, sizeof(hdr) + payload_size);
	memcpy(data, &hdr, sizeof(hdr));

    return hdr;
}

void    send_socket(struct in_addr target_ip, int sockfd, size_t payload_size, int sequence)
{
	struct sockaddr_in sockaddr;
    sockaddr = get_sockaddr(target_ip);

	unsigned char data[1024];

    build_icmp_header(data, sequence, payload_size);

	int sent_bytes = 0;
	sent_bytes = sendto(sockfd, data, sizeof(struct icmphdr) + payload_size, 0, (struct sockaddr *) &sockaddr, sizeof(sockaddr));

	if (sent_bytes == -1)
	{
		perror("sendto");
		return ;
	}

	pthread_mutex_lock(&g_stats_mutex);
	save_send_time(g_stats, sequence);
	g_stats->sent_sockets++;
	pthread_mutex_unlock(&g_stats_mutex);
}