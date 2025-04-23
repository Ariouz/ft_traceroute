#include "ft_ping.h"

int main(void)
{
	t_opts *opts;

	opts = init_cli_options();

	print_help(opts);

	return 0;

	char *target = "google.com";
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
	rc = sendto(sockfd, data, sizeof(hdr) + 5, 0, (struct sockaddr *) &sockaddr, sizeof(sockaddr));

	if (rc == -1)
	{
		perror("sendto");
		return 1;
	}

	printf("sent socket\n");

	struct icmphdr rcv_hdr;
	struct timeval timeout = {3, 0};

	fd_set read_set;
	memset(&read_set, 0, sizeof(read_set));
	FD_SET(sockfd, &read_set);

	rc = select(sockfd + 1, &read_set, NULL, NULL, &timeout);
	printf("rc: %d\n", rc);
	if (rc == -1)
	{
		perror("select");
		return -1;
	}
	if (rc == 0)
	{
		printf("timeout\n");
		return -1;
	}

	socklen_t slen;

	slen = 0;
	rc = recvfrom(sockfd, data, sizeof(data), 0, NULL, &slen);

	if (rc < 0)
	{
		perror("recvfrom");
		return -1;
	}

	if (rc < sizeof(rcv_hdr))
	{
		printf("some packet data is lost\n");
		return -1;
	}

	memcpy(&rcv_hdr, data, sizeof(rcv_hdr));
	if (rcv_hdr.type == ICMP_ECHOREPLY){
		printf("got reply from echo %d; sequence %d\n", rcv_hdr.un.echo.id, rcv_hdr.un.echo.sequence);
	}
	else {
		// if verbose, print
		return -1;
	}

	FD_CLR(sockfd, &read_set);
	close(sockfd);
	return (1);
}
