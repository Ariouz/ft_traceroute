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
# include <stdbool.h>
# include <bits/getopt_posix.h>

# define _USE_POSIX2_

# define MAX_OPTS 64

# define OPT_HELP '?'
# define OPT_VERBOSE 'v'

typedef struct s_option {
    char    code;
    char    *description;
    void    *value;
    bool    requires_value;
}   t_option;

typedef struct s_opts {
    t_option        **options;
    size_t          size;
    char            *hostname;
}   t_opts;


struct in_addr  resolve_ip(const char *target);
char            *to_str(const struct in_addr addr);

void            print_help(t_opts *opts);


t_opts          *init_cli_options();
t_option        *create_option(int code, char *description, void *default_value, bool requires_value);
void            add_option(int code, char *description, void *default_value, bool requires_value, t_opts *opts);
void            save_option(t_opts *opts, t_option *option);

bool            set_option(t_opts *opts, int opt, char *optarg);
t_option        *get_option(int code, t_opts *opts);

int             parse_cli_options(t_opts *opts, int argc, char **argv);
// int             is_valid_option(const char* opt);


void            clean_free(t_opts *opts);
void            free_options(t_option **options, size_t opt_len);

void            exit_error(t_opts *opts);
void            fatal_error(char *msg, t_opts *opts);
void            error_required_optval(char *alias, t_opts *opts);

#endif
