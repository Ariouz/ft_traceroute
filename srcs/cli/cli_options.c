# include "ft_traceroute.h"


t_option    *get_option(int code)
{
    size_t      index;
    index = 0;

    while (index < g_opts->size)
    {
        if (code == g_opts->options[index]->code)
            return g_opts->options[index];
        index++;
    }

    return NULL;
}

void print_help()
{
    size_t  index;
    printf("Usage: ft_traceroute [options] <destination>\nOptions:\n");

    index = 0;
    while (index < g_opts->size) {
        t_option *opt = g_opts->options[index];

        printf("\t -%c", opt->code);
        if (opt->requires_value) printf(" <value>");
        printf("\t\t\t%s\n", opt->description);

        index++;
    }

    clean_free(g_opts);
    exit(0);
}