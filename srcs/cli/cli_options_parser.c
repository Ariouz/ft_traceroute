#include "ft_ping.h"

bool    process_option(t_opts *opts, t_option *option, char *value)
{
    (void) value;
    if (option->code == OPT_HELP)
        print_help(opts);
    if (option->code == OPT_VERBOSE)
        option->value = (void *) true;

    return true; // todo return false if 'value' if value is invalid
}

int     parse_cli_options(t_opts *opts, int argc, char **argv)
{
    (void) opts;
    (void) argc;
    (void) argv;

    // todo parse using getopt, optarg, optopt
   
    return 0;
}



bool    set_option(t_opts *opts, int opt, char *optarg)
{
    t_option *option;

    option = get_option(opt, opts);
    if (!option)
        fatal_error("Invalid option", opts);

    if (opt == '?' && optopt != 0)
        exit_error(opts);

    return process_option(opts, option, optarg);
}