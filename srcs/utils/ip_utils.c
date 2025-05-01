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

void print_ip_hdr_dump(struct iphdr *ip)
{
    unsigned char *raw = (unsigned char *)ip;
    size_t len = ip->ihl * 4;

    printf("IP Hdr Dump:\n");
	printf(" ");
    for (size_t i = 0; i < len; i++) {
        printf("%02x", raw[i]);
        if ((i + 1) % 2 == 0)
            printf(" ");
    }
    printf("\n");
}


void print_verbose(struct iphdr *inner_ip, struct icmphdr *inner_icmp)
{
	print_ip_hdr_dump(inner_ip);
		printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src      Dst     Data\n");
    	printf("%2d %2d  %02x %04x %04x   %d %04x  %02x  %02x %04x %s  %s\n", 
			inner_ip->version,
			inner_ip->ihl,
			inner_ip->tos,
           	ntohs(inner_ip->tot_len),
		   	ntohs(inner_ip->id),
           	(ntohs(inner_ip->frag_off) & 0xE000) >> 13,
        	ntohs(inner_ip->frag_off) & 0x1FFF,
           	inner_ip->ttl,
           	inner_ip->protocol,
           	inner_icmp->checksum,
           	inet_ntoa(*(struct in_addr*)&inner_ip->saddr),
           	inet_ntoa(*(struct in_addr*)&inner_ip->daddr));
		printf("ICMP: type %d, code %d, size %d, id 0x%04x, seq 0x%04x\n",
			inner_icmp->type, inner_icmp->code, ntohs(inner_ip->tot_len) - (inner_ip->ihl * 4), inner_ip->id, inner_icmp->un.echo.sequence);
		
}