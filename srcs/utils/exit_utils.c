#include "ft_ping.h"

void    clean_free(t_opts *opts)
{
    if (!opts) return;

    free_options(opts->options, opts->size);
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
