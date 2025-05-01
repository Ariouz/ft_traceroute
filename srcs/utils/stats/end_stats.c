#include "ft_ping.h"

void   get_avg_rtt()
{
    float total_rtt = 0;
    size_t  index;

    index = 0;
    while (index < g_stats->sock_rtt_size)
    {
        total_rtt += g_stats->sock_rtt[index].rtt;
        index++;
    }

    if (g_stats->rcvd_sockets == 0)
    {
        g_stats->avg_rtt = 0;
        return ;
    }
    g_stats->avg_rtt = (float) (total_rtt / (float) g_stats->rcvd_sockets);
}

void    get_stddev()
{
    size_t  index = 0;
    float   variance = 0;

    while (index < g_stats->sock_rtt_size)
    {
        variance += ft_pow(g_stats->sock_rtt[index].rtt - g_stats->avg_rtt, 2);
        index++;
    }

    variance = variance / g_stats->sock_rtt_size; 
    g_stats->stddev = ft_sqrt(variance);
}

void    print_stats()
{
    get_avg_rtt();
    get_stddev();

    printf("--- %s ping statistics ---\n", g_opts->hostname);
    printf("%ld packets transmitted, %ld packets received, %ld%% packet lost\n", 
        g_stats->sent_sockets, g_stats->rcvd_sockets, (1 - (g_stats->rcvd_sockets/g_stats->sent_sockets)) * 100);
    if (g_stats->avg_rtt > 0)
        printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", 
        g_stats->min_rtt, g_stats->avg_rtt, g_stats->max_rtt, g_stats->stddev);
}
