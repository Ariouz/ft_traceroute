#include "ft_ping.h"

void    exit_error(t_opts *opts)
{
    clean_free(opts);
    exit(1);
}

void    fatal_error(char *msg, t_opts *opts)
{
    printf("Error: %s\n", msg);
    exit_error(opts);
}

void    error_required_optval(char *alias, t_opts *opts)
{
    if (!alias) fatal_error("Fatal: cli alias error", opts);
    char    *msg = "Missing option value: ";
    char    *errmsg = malloc(sizeof(char) * (strlen(alias) + strlen(msg) + 1));

    if (!errmsg)
        fatal_error("Fatal: allocation error", opts);
    
    sprintf(errmsg, "Missing option value: %s", alias);
    fatal_error(errmsg, opts);
}