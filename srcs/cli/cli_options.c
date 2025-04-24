# include "ft_ping.h"


t_option    *get_option(int code, t_opts *opts)
{
    size_t      index;
    
    index = 0;

    while (index < opts->size)
    {
        if (code == opts->options[index]->code)
            return opts->options[index];
        index++;
    }

    return NULL;
}

void print_help(t_opts *opts)
{
    printf("Usage: ft_ping [options] <destination>\nOptions:\n");

    for (size_t i = 0; i < opts->size; i++) {
        t_option *opt = opts->options[i];

        printf("\t -%c", opt->code);
        if (opt->requires_value) printf(" <value>");
        printf("\t\t\t%s\n", opt->description);
    }

    clean_free(opts);
    exit(0);
}