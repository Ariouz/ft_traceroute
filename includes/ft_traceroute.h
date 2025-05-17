#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H
# define __USE_POSIX2

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
# include <errno.h>
# include <getopt.h>
# include <signal.h>

# define true 1
# define false 0

# define SOCK_RTT_BATCH_SIZE 10

# define OPT_HELP '?'
# define OPT_REPLY_COUNT 'c'
# define OPT_INTERVAL 'i'
# define OPT_QUIET 'q'
# define OPT_PACKET_SIZE 's'
# define OPT_TOS 'T'
# define OPT_VERBOSE 'v'
# define OPT_LINGER 'W'

# define MAX_OPTS 16

# define SOCK_SENT 0
# define SOCK_RECEIVED 1
# define SOCK_TIMEOUT 2


typedef int bool;

typedef struct s_sock_rtt {
    struct timeval      start;
    struct timeval      end;
    int                 sequence;
    float               rtt;
    int                 status;
}   t_sock_rtt;

typedef struct s_stats {
    t_sock_rtt  *sock_rtt;
    size_t      sock_rtt_size;
    size_t      sock_rtt_total_size;

}   t_stats;

typedef struct s_option {
    char    code;
    char    *description;
    int     value;
    bool    requires_value;
}   t_option;

typedef struct s_opts {
    t_option        **options;
    size_t          size;
    char            *hostname;
    bool            is_running;
    int             sockfd;
    struct in_addr  target_ip;
}   t_opts;


extern t_opts *g_opts;
extern t_stats *g_stats;


struct in_addr  resolve_ip(const char *target);
char            *to_str(const struct in_addr addr);
unsigned short  checksum(void *data, int len);

void            handle_signals();

void            *sender_routine(void *arg);
void            *receiver_routine(void *arg);
void            check_timeouts();

void            init_cli_options();
t_option        *create_option(int code, char *description, int default_value, bool requires_value);
void            add_option(int code, char *description, int default_value, bool requires_value);
void            save_option(t_option *option);

bool            set_option(int opt, char *optarg);
t_option        *get_option(int code);
void            parse_cli_options(int argc, char **argv);
int             parse_opt_int(char *value, int min, int max);


int                 open_socket();
struct sockaddr_in  get_sockaddr(struct in_addr target_ip);
void                send_socket(struct in_addr target_ip, int sockfd, size_t payload_size, int sequence);
size_t              wait_socket(int sockfd, fd_set *read_set);
void                receive_socket(int sockfd, fd_set *read_set);
void	            handle_icmp_types(struct sockaddr_in rcv_sockaddr, struct icmphdr *rcv_icmp, size_t rcv_bytes, struct iphdr *rcv_ip);

void            print_help();

void            clean_free();
void            free_options(t_option **options, size_t opt_len);

bool            is_runnning();

void            exit_error();
void            fatal_error(char *msg);
void            clean_exit(int code);

t_stats         *init_stats();
void            save_send_time(t_stats *stats, int sequence);
float           save_rcv_time(t_stats *stats, int sequence);
t_sock_rtt      *get_sock_rtt(t_stats *stats, int sequence);

long long       get_time_ms(struct timeval tv);
long long       get_time_us(struct timeval tv);
int		        multisleep(long max_time_millis, long increment);

void            trace();

#endif
