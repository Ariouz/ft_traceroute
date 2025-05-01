#include "ft_ping.h"

bool    process_option(t_option *option, char *value)
{
    (void) value;
    if (option->code == OPT_HELP)
        print_help();
    if (option->code == OPT_VERBOSE)
        option->value = true;
    if (option->code == OPT_QUIET)
        option->value = true;
    if (option->code == OPT_REPLY_COUNT)
        option->value = parse_opt_int(value, 1, INT32_MAX);
    if (option->code == OPT_LINGER)
        option->value = parse_opt_int(value, 1, INT32_MAX);
    if (option->code == OPT_INTERVAL)
        option->value = parse_opt_int(value, 1, INT32_MAX);
    if (option->code == OPT_PACKET_SIZE)
        option->value = parse_opt_int(value, 1, 1000);
    if (option->code == OPT_TOS)
        option->value = parse_opt_int(value, 0, 255);

    return true;
}

void     parse_cli_options(int argc, char **argv)
{
    int opt;
	while ((opt = getopt(argc, argv, "?c:i:qs:T:vW")) != -1)
		set_option(opt, optarg);
   
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

int parse_opt_int(char *value, int min, int max)
{
    char *endptr;
    long val = 0;
    errno = 0;

    val = strtol(value, &endptr, 10);

    if (endptr == value)
        fatal_error("Option value must be a valid integer");

    if (errno != 0 && val == 0)
        fatal_error(strerror(errno));

    if (val < min)
        fatal_error("Option value is too low");

    if (val > max)
        fatal_error("Option value is too big");

    return (int) val;
}