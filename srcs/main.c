#include "ft_ping.h"

int main(void)
{
	char *target = "az-projects.xyz";
	struct in_addr target_ip;

	target_ip = resolve_ip(target);
	if (target_ip.s_addr == INADDR_NONE)
		return 1;

	printf("Pinging IP: %s\n", to_str(target_ip));

	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sockfd == -1)
	{
		perror("socket");
		return 1;
	}

	struct sockaddr_in sockaddr;
	sockaddr.sin_addr = target_ip;
	sockaddr.sin_family = AF_INET;

	struct icmphdr hdr;
	hdr.type = ICMP_ECHO;
	hdr.un.echo.id = 1234;

	int sequence = 0;
	hdr.un.echo.sequence = sequence++;

	unsigned char data[2048];
	memcpy(data, &hdr, sizeof(hdr));
	memcpy(data + sizeof(hdr), "Test", 4);

	int rc = 0;
	rc = sendto(sockfd, data, sizeof(hdr) + 5, 0, &sockaddr, sizeof(sockaddr));

	if (rc == -1)
	{
		perror("sendto");
		return 1;
	}

	printf("sent socket");

	struct icmphdr rcv_hdr;
	struct timeval timeout = {3, 0};

	fd_set read_set;
	FD_ZERO(&read_set);
	FD_SET(sockfd, &read_set);
	// select()
	// recv_from

	return (1);
}
