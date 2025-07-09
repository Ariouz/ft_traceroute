#include "ft_traceroute.h"

# ifdef BONUS
bool    process_option(t_option *option, char *value)
{
    if (option->code == OPT_FIRST_TTL
        || option->code == OPT_MAX_TTL
        || option->code == OPT_SOCK_TIMEOUT)
        option->value = parse_opt_int(value, 1, INT32_MAX);
    
    else if (option->code == OPT_NO_DNS)
        option->value = 1;

    else if (option->code == OPT_PORT)
        option->value = parse_opt_int(value, 1, 65535);

    return true;
}
# endif

void     parse_cli_options(int argc, char **argv)
{

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0)
            print_help();
    }

    # ifdef BONUS
    int opt;
	while ((opt = getopt(argc, argv, "m:p:f:w:n")) != -1)
		set_option(opt, optarg);
    # endif
   
}

# ifdef BONUS
bool    set_option(int opt, char *optarg)
{
    t_option *option;

    option = get_option(opt);
    if (!option)
        fatal_error("Invalid option, use --help for options list");

    return process_option(option, optarg);
}
# endif

# ifdef BONUS
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
# endif