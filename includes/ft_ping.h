#ifndef FT_PING_H
# define FT_PING_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <sys/socket.h>

# define MAX_OPTS 64

# define OPT_HELP 0
# define OPT_VERBOSE 1

typedef struct s_opt_alias
{
    int     code;
    char    *alias;
}   t_opt_alias;

typedef struct s_option {
    int     code;
    char    *alias;
    char    *description;
    void    *value;
}   t_option;

typedef struct s_opts {
    t_option        **options;
    t_opt_alias     **aliases;
    size_t          size;
    size_t          alias_size;
    char            *hostname;
}   t_opts;


struct in_addr  resolve_ip(const char *target);
char            *to_str(const struct in_addr addr);

void            print_help(t_opts *opts);
void            print_aliases(t_opts *opts);

t_opt_alias     **init_aliases();
void            add_opt_alias(t_opts *opts, int opt_code, char *alias);

t_opts          *init_cli_options();
t_option        *create_option(char *alias, char *description, void *default_value, int code);
void            add_option(char *alias, char *description, void *default_value, int code, t_opts *opts);
void            save_option(t_opts *opts, t_option *option);
int             is_valid_option(const char* opt);

#endif
