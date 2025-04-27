#include "ft_ping.h"

t_opts *g_opts = NULL;
t_stats *g_stats = NULL;
pthread_mutex_t g_opts_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t g_stats_mutex = PTHREAD_MUTEX_INITIALIZER;

void	run()
{
	pthread_t sender_thread, receiver_thread;
	printf("PING %s (%s): %d data bytes", g_opts->hostname, to_str(g_opts->target_ip), 56);
	if (get_option(OPT_VERBOSE)->value == true)
		printf(" id 0x%x = %d", getpid() & 0xFFFF, getpid());
	printf("\n");

	pthread_create(&sender_thread, NULL, sender_routine, NULL);
	pthread_create(&receiver_thread, NULL, receiver_routine, NULL);

	pthread_join(sender_thread, NULL);
	pthread_join(receiver_thread, NULL);
	pthread_mutex_destroy(&g_opts_mutex);
	pthread_mutex_destroy(&g_stats_mutex);
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
	run();

    print_stats();
	clean_exit(0);
	return (0);
}
