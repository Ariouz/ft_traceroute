# include "ft_ping.h"


t_option    *get_option(char *alias, t_opts *opts)
{
    t_opt_alias **aliases;
    size_t      index;
    
    aliases = opts->aliases;
    index = 0;

    while (index < opts->alias_size)
    {
        if (strcmp(alias, aliases[index]->alias) == 0)
            break ;
        index++;
    }

    t_opt_alias *opt_alias = aliases[index];
    index = 0;

    for (size_t opt_index = 0; opt_index < opts->size; opt_index++) {
        if (opts->options[opt_index]->code == opt_alias->code)
            return  opts->options[opt_index];
    }

    return NULL;
}

void print_help(t_opts *opts)
{
    printf("Usage: ft_ping [options] <destination>\nOptions:\n");

    int alias_count;
    for (size_t i = 0; i < opts->size; i++) {
        t_option *opt = opts->options[i];

        alias_count = 0;
        for (size_t alias_index = 0; alias_index < opts->alias_size; alias_index++) {
            if (opts->aliases[alias_index]->code == opt->code)
            {
                if (alias_count > 0)
                printf(", ");
                printf("%s", opts->aliases[alias_index]->alias);
                if (opt->requires_value) printf(" - value");
                alias_count++;
            }
        }

        printf(":\t\t%s\n", opt->description);
    }
}