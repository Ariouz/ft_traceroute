#include "ft_ping.h"

bool    process_option(t_option *option, char *value)
{
    (void) value;
    if (option->code == OPT_HELP)
        print_help();
    if (option->code == OPT_VERBOSE)
        option->value = true;

    return true;
}

void     parse_cli_options(int argc, char **argv)
{
    int opt;
	while ((opt = getopt(argc, argv, "?v")) != -1)
	{
		//printf("getopt: %d, arg: %s\n", opt, optarg);
		set_option(opt, optarg);
	}
   
}

bool    set_option(int opt, char *optarg)
{
    t_option *option;

    option = get_option(opt);
    if (!option)
        fatal_error("Invalid option, use -? for options list");

    if (opt == '?' && optopt != 0)
        exit_error();

    return process_option(option, optarg);
}