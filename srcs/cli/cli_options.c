# include "ft_ping.h"

void    parse_options(int argc, char **argv, t_opts *opts)
{
    int index = 0;
    while (index < argc)
    {
        // todo parse
        index++;
    }
}

int is_valid_option(const char *opt)
{
    return 0;
}

void    *get_option(char *identifier)
{
    return 0;
}

void    print_help(t_opts *opts)
{
    int index;

    index = 0;
    while (index < opts->size)
    {
        t_option *option = opts->options[index];
        print_aliases(opts);
        printf(": %s \n", option->description);
        index++;
    }
}

void    print_aliases(t_opts *opts)
{
    int index;

    index = 0;
    while (index < opts->alias_size)
    {
        printf("%s\n", opts->aliases[index]->alias);
        index++;
    }
}