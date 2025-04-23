#include "ft_ping.h" 

struct in_addr	resolve_ip(const char *hostname)
{
	struct in_addr address;
	struct hostent *he;

	address.s_addr = INADDR_NONE;

	if (!inet_aton(hostname, &address)) { // Invalid IPv4
		he = gethostbyname(hostname);
		if (!he || he->h_addrtype != AF_INET) {
			fprintf(stderr, "Error: unknown host\n"); // todo error function
			return address;
		}

		address = *(struct in_addr *)he->h_addr_list[0];
	}
	return address;
}

char    *to_str(const struct in_addr addr)
{
    if (!&(addr.s_addr)) return "Unknown";
    return inet_ntoa(addr);
}