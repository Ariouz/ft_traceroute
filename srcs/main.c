#include "ft_ping.h"


unsigned short checksum(void *b, int len) {
	unsigned short *buf = b;
	unsigned int sum = 0;
	unsigned short result;

	for (sum = 0; len > 1; len -= 2)
		sum += *buf++;
	if (len == 1)
		sum += *(unsigned char *)buf;

	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}

int main(void)
{
	t_opts *opts;

	opts = init_cli_options();

	print_help(opts);

	// return 0;

	char *target = "1.1.1.1";
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
	hdr.un.echo.id = getpid() & 0xFFFF;
	hdr.code = 0;

	int sequence = 0;
	hdr.un.echo.sequence = sequence++;

	unsigned char data[1024];
	memcpy(data, &hdr, sizeof(hdr));

	char payload[56];
	memset(&payload, 'a', 56);
	memcpy(data + sizeof(hdr), payload, 56);

	hdr.checksum = checksum(data, sizeof(hdr) + 56);
	memcpy(data, &hdr, sizeof(hdr));


	int rc = 0;
	rc = sendto(sockfd, data, sizeof(hdr) + 56, 0, (struct sockaddr *) &sockaddr, sizeof(sockaddr));

	if (rc == -1)
	{
		perror("sendto");
		return 1;
	}

	printf("sent socket\n");

	struct icmphdr rcv_hdr;
	struct timeval timeout = {3, 0};

	char	rcv_buf[1024];

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

	slen = sizeof(struct sockaddr_in);
	rc = recvfrom(sockfd, rcv_buf, sizeof(rcv_buf), 0, (struct sockaddr *) &sockaddr, &slen);

	if (rc < sizeof(struct iphdr) + sizeof(struct icmphdr)) {
		printf("some packet data is lost\n");
		return -1;
	}
	
	struct iphdr *ip = (struct iphdr *)rcv_buf; 
	int ip_header_len = ip->ihl * 4;

	struct icmphdr *rcv_icmp = (struct icmphdr *)(rcv_buf + sizeof(struct iphdr));

	
	if (rcv_icmp->type == ICMP_ECHOREPLY) {
		printf("got reply from echo %d; sequence %d; bytes %d\n", rcv_icmp->un.echo.id, rcv_icmp->un.echo.sequence, rc-ip_header_len);
	} else {
		printf("Received ICMP type %d (not echo reply)\n", rcv_icmp->type);
	}

	FD_CLR(sockfd, &read_set);
	close(sockfd);
	return (1);
}
