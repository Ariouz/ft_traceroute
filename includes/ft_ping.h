#ifndef FT_PING_H
# define FT_PING_H

# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>

struct in_addr  resolve_ip(char *target);
char            *to_str(struct in_addr addr);

#endif
