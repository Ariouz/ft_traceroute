#include "ft_ping.h" 

struct in_addr	resolve_ip(const char *hostname)
{
	struct in_addr address;
	struct hostent *he;

	address.s_addr = INADDR_NONE;

	if (!inet_aton(hostname, &address)) { // Invalid IPv4
		he = gethostbyname(hostname);
		if (!he || he->h_addrtype != AF_INET)
			return address;

		address = *(struct in_addr *)he->h_addr_list[0];
	}
	return address;
}

char    *to_str(const struct in_addr addr)
{
    return inet_ntoa(addr);
}

unsigned short checksum(void *data, int len)
{
	unsigned short *buf = data;
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