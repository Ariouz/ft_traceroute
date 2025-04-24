#include "ft_ping.h"

t_opts      *init_opts()
{
    t_opts  *opts;
    opts = (t_opts *) malloc(sizeof(t_opts));
    if (!opts)
        return NULL;
    
    opts->options = malloc(sizeof(t_option *) * MAX_OPTS);
    if (!opts->options)
        return NULL;

    opts->size = 0;
    opts->hostname = NULL;

    return opts;
}

t_opts    *init_cli_options()
{
    t_opts *opts;
    opts = init_opts();
    if (!opts)
    {
        fatal_error("Fatal: Failed to init CLI options", opts);
        return NULL;
    }

    add_option(OPT_VERBOSE, "Enable verbose", false, true, opts);
    add_option(OPT_HELP, "Display this help list", false, false, opts);

    return (opts);
}

void        add_option(int code, char *description, void *default_value, bool requires_value, t_opts *opts)
{
    t_option   *option;

    option = create_option(code, description, default_value, requires_value);
    if (!option)
    {
        fatal_error("Fatal: Failed to create CLI option", opts);
        return ;
    }
    save_option(opts, option);
}

void        save_option(t_opts *opts, t_option *option)
{
    if (opts->size >= MAX_OPTS) return ;
    opts->options[opts->size] = option;
    opts->size++;
}

t_option    *create_option(int code, char *description, void *default_value, bool requires_value)
{
    t_option *option;
    option = (t_option *) malloc(sizeof(t_option));
    if (!option)
        return NULL;

    option->code = code;
    option->description = description;
    option->value = default_value;
    option->requires_value = requires_value;

    return option;
}
