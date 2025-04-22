#include "ft_ping.h"

int main(void)
{
	char *target = "az-projects.xyz";
	struct in_addr target_ip;

	target_ip = resolve_ip(target);
	if (!target_ip.s_addr)
		return 0;

	printf("Pinging IP: %s\n", to_str(target_ip));

	// if ((int fd = socket(AF_INET, ,IPPROTO_TCP)) == -1)

	return (0);
}
