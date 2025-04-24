#include "ft_ping.h"

void    clean_free(t_opts *opts)
{
    if (!opts) return;

    free_options(opts->options, opts->size);
    free_aliases(opts->aliases, opts->alias_size);

    free(opts);
}

void    free_options(t_option **options, size_t opt_len)
{
    size_t  index = 0;
    if (!options) return ;

    while (index < opt_len)
    {
        free(options[index]);
        index++;
    }
    free(options);
}


void    free_aliases(t_opt_alias **aliases, size_t aliases_len)
{
    size_t  index = 0;
    if (!aliases) return ;

    while (index < aliases_len)
    {

        free(aliases[index]);
        index++;
    }
    free(aliases);
}