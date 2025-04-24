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

    t_opt_alias **aliases;
    aliases = init_aliases();

    if (!aliases)
        return NULL;

    opts->aliases = aliases;
    opts->alias_size = 0;

    return opts;
}

t_opt_alias **init_aliases()
{
    t_opt_alias **aliases;

    aliases = (t_opt_alias **) malloc(sizeof(t_opt_alias *));
    aliases[0] = 0;
    return aliases;
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
    add_opt_alias(opts, OPT_VERBOSE, "-v");
    add_opt_alias(opts, OPT_VERBOSE, "--verbose");

    add_option(OPT_HELP, "Display this help list", false, false, opts);
    add_opt_alias(opts, OPT_HELP, "-?");
    add_opt_alias(opts, OPT_HELP, "--help");

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

void add_opt_alias(t_opts *opts, int opt_code, char *alias)
{
    t_opt_alias *new_alias = malloc(sizeof(t_opt_alias));
    if (!new_alias) return;

    new_alias->code = opt_code;
    new_alias->alias = alias;

    t_opt_alias **new_aliases = malloc(sizeof(t_opt_alias *) * (opts->alias_size + 1));
    if (!new_aliases) return;

    for (size_t i = 0; i < opts->alias_size; i++)
        new_aliases[i] = opts->aliases[i];
    new_aliases[opts->alias_size] = new_alias;

    free(opts->aliases);
    opts->aliases = new_aliases;
    opts->alias_size++;
}
