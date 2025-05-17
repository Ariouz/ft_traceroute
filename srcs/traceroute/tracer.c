#include "ft_traceroute.h"

void print_hostname_and_ip(struct in_addr addr)
{
    struct hostent *host = gethostbyaddr(&addr, sizeof(addr), AF_INET);
    if (host && host->h_name)
        printf("%s (%s)", host->h_name, inet_ntoa(addr));
    else
        printf("%s (%s)", inet_ntoa(addr), inet_ntoa(addr));
}

void    process_tll(int ttl, int payload_size, int sockfd)
{
    fd_set read_set;
    struct s_requests {
        float rtt;
        struct sockaddr_in responder;
    }  requests[3];

    struct sockaddr_in last_responder;

    for (int request_no = 0; request_no < 3; request_no++)
    {
        if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
        fatal_error(strerror(errno));
        send_socket(g_opts->target_ip, sockfd, payload_size, 0);
        requests[request_no].rtt = -1;
        
        struct timeval start;
        gettimeofday(&start, NULL);
        
        FD_ZERO(&read_set);
        FD_SET(sockfd, &read_set);
        
        size_t bytes = wait_socket(sockfd, &read_set);
        if (bytes == (size_t) -1)
            continue ;
        
        if (!FD_ISSET(sockfd, &read_set)) return ;
        FD_CLR(sockfd, &read_set);
        
        char			rcv_buf[1024];
        socklen_t slen;
        
        slen = sizeof(struct sockaddr_in);
        size_t rcv_bytes = recvfrom(sockfd, rcv_buf, sizeof(rcv_buf), 0, (struct sockaddr *) &requests[request_no].responder, &slen);
        last_responder = requests[request_no].responder;
        
        if (rcv_bytes < (size_t) (sizeof(struct iphdr) + sizeof(struct icmphdr))) return ;
        
        struct icmphdr *rcv_icmp = (struct icmphdr *)(rcv_buf + sizeof(struct iphdr));
        
        struct timeval end;
        gettimeofday(&end, NULL);
        float rtt = (get_time_us(end) - get_time_us(start)) / (float) 1000.0;
        requests[request_no].rtt = rtt;
        
        if (rcv_icmp->type == ICMP_ECHOREPLY) {
            //printf("Traceroute end at req %d ttl %d\n", request_no, ttl);
            g_opts->is_running = false;
            break;
        }
       
        if (rcv_icmp->type != ICMP_TIME_EXCEEDED) continue;
        if (rcv_icmp->code != ICMP_EXC_TTL) continue;
    }

    int responded = 0;
    for (int i = 0; i < 3; i++) {
        if (requests[i].rtt >= 0.0) {
            responded = 1;
            break;
        }
    }

    printf("%2d  ", ttl);
    if (responded)
    {
        print_hostname_and_ip(last_responder.sin_addr);
        printf("  ");
    }

    for (int i = 0; i < 3; i++) {
        if (requests[i].rtt < 0.0)
            printf("* ");
        else
            printf("%.3f ms  ", requests[i].rtt);
    }

    printf("\n");
}

void    trace()
{
    int sockfd = g_opts->sockfd;
    int max_ttl = 30;
    int ttl = 1;
    int payload_size = 60;

    printf("ft_traceroute to %s (%s), %d hops max, %d byte packets\n",
       g_opts->hostname, to_str(g_opts->target_ip), max_ttl, payload_size);


    while (ttl < max_ttl && g_opts->is_running)
    {
        process_tll(ttl, payload_size, sockfd);

        ttl++;
    }

}