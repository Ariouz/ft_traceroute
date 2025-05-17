#include "ft_traceroute.h"

t_opts *g_opts = NULL;
t_stats *g_stats = NULL;

void	run()
{
	printf("PING %s (%s): %d data bytes", g_opts->hostname, to_str(g_opts->target_ip), 56);
	if (get_option(OPT_VERBOSE)->value == true)
		printf(" id 0x%x = %d", getpid() & 0xFFFF, getpid());
	printf("\n");
}

int main(int argc, char **argv)
{
	init_cli_options(argc, argv);
	parse_cli_options(argc, argv);
	g_stats = init_stats();

	int args_count = argc - optind;
	if (args_count < 1)
		fatal_error("Destination IP/hostname required");
	if (args_count > 1)
		fatal_error("Destination already set");

	g_opts->hostname = argv[optind];
	g_opts->target_ip = resolve_ip(argv[optind]);

	if (g_opts->target_ip.s_addr == INADDR_NONE)
		fatal_error("Unknown host");

	handle_signals();
	g_opts->sockfd = open_socket();
	//run();
	trace();
	clean_exit(0);
	return (0);
}
