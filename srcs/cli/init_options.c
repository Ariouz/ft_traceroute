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
        // todo error / exit
        return NULL;
    }


    add_option("-v", "Enable verbose", 0, OPT_VERBOSE, opts);
    add_opt_alias(opts, OPT_VERBOSE, "--verbose");

    return (opts);
}

void        add_option(char *alias, char *description, void *default_value, int code, t_opts *opts)
{
    t_option   *option;

    option = create_option(alias, description, default_value, code);
    if (!option)
    {
        // todo error / exit
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

t_option    *create_option(char *alias, char *description, void *default_value, int code)
{
    t_option *option;
    option = (t_option *) malloc(sizeof(t_option));
    if (!option)
        return NULL;

    option->alias = alias;
    option->description = description;
    option->value = default_value;
    option->code = code;

    return option;
}

void    add_opt_alias(t_opts *opts, int opt_code, char *alias)
{
    t_opt_alias *new_alias;
    new_alias = (t_opt_alias *) malloc(sizeof(t_opt_alias));
    new_alias->code = opt_code;
    new_alias->alias = alias;

    t_opt_alias **tmp = opts->aliases;

    opts->aliases = (t_opt_alias **) malloc(sizeof(t_opt_alias *) * opts->alias_size + 1);
    memcpy(opts->aliases, &tmp, sizeof(tmp));
    opts->aliases[++opts->alias_size] = new_alias;
}