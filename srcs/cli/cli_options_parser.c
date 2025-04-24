#include "ft_ping.h"

int    is_valid_option(char *arg, t_opts *opts)
{
    t_opt_alias **aliases;
    size_t      index;
    
    aliases = opts->aliases;
    index = 0;

    while (index < opts->alias_size)
    {
        if (strcmp(arg, aliases[index]->alias) == 0)
            return 0;
        index++;
    }

    return -1;
}

int     process_invalid_option(char *arg, t_opts *opts)
{
    printf("hostname: %s\n", opts->hostname);
    if (arg[0] == '-')
    {
        printf("Invalid option: %s\n", arg);
        return -1;
    }

    if (opts->hostname != NULL)
    {
        printf("hostname already defined");    
        return -1;
    }
    // todo check and set hostname
    printf("set hostname\n");
    return 0;
}

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
    int index;

    index = 1;
    while (index < argc)
    {
        if (is_valid_option(argv[index], opts) == -1)
        {
            if (process_invalid_option(argv[index], opts) == -1)
                return -1;
        }

        t_option *option = get_option(argv[index], opts);
        char     *value = NULL;
        if (option->requires_value)
        {
            if (index + 1 == argc || is_valid_option(argv[index + 1], opts) == 0)
                error_required_optval(argv[index], opts);
            index++;
            value = malloc(sizeof(char) * strlen(argv[index]));
            strcpy(value, argv[index]);
        }
        
        if (process_option(opts, option, value) == false)
            return -1;

        if (value) free(value);
        index++;
    }
    return 0;
}
