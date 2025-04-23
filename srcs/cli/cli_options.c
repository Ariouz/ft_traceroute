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

void print_help(t_opts *opts)
{
    printf("Usage: ft_ping [options] <destination>\nOptions:\n");

    for (size_t i = 0; i < opts->size; i++) {
        t_option *opt = opts->options[i];
        printf("  %s", opt->alias);

        for (size_t alias_index = 0; alias_index < opts->alias_size; alias_index++) {
            if (opts->aliases[alias_index]->code == opt->code)
                printf(", %s", opts->aliases[alias_index]->alias);
        }

        printf(":\t%s\n", opt->description);
    }
}